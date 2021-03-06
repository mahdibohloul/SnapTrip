#include "Backend.hpp"
#include "../ORM/ObjectRelational.hpp"
#include "../ConstNames.hpp"
#include "../Exception/Exception.hpp"
#include "../API/Result/Result.hpp"
#include "../Models/Rating.hpp"
#include "./AI/AI.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <regex>
#include <numeric>
using namespace std;

Backend* Backend::instance = nullptr;

Backend* Backend::get_instance()
{
    if(instance == nullptr){
        instance = new Backend();
    }
    return instance;
}

void Backend::release()
{
    delete instance;
    instance = nullptr;
}

Backend::Backend()
{
    object_relational = nullptr;
    online_users = Database::l_users();
    construct_maps();
}

Backend::~Backend()
{
    online_users.clear();
}


Backend::data_t Backend::parse_content(Content content, char separator)
{
    data_t parsed_data;
    int separator_pos;
    while((separator_pos = content.find(separator)) != string::npos)
    {
        parsed_data.push_back(content.substr(0, separator_pos));
        content = content.substr(separator_pos + 1);
    }
    parsed_data.push_back(content.substr(separator_pos + 1));
    return parsed_data;
}

void Backend::command_processor(data_t command_data)
{
    if(!command_data.empty())
    {
        object_relational = ObjectRelational::get_instance();
        auto command_itr = command_data.begin();
        if(*command_itr != ConstNames::POST_METHOD && *command_itr != ConstNames::GET_METHOD && *command_itr != ConstNames::DELETE_METHOD)
            throw Exception(ConstNames::Bad_Request_msg);
        request_method method = func_method_map[*command_itr];
        command_data.erase(command_itr);
        (this->*method)(command_data);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_request(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(in_the_command(*command_itr, POST) != IN)
            throw Exception(ConstNames::Not_Found_msg);
        request_method method = func_post_map[*command_itr];
        data.erase(command_itr);
        (this->*method)(data);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::get_request(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(in_the_command(*command_itr, GET) != IN)
            throw Exception(ConstNames::Not_Found_msg);
        request_method method = func_get_map[*command_itr];
        data.erase(command_itr);
        (this->*method)(data);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::delete_request(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(in_the_command(*command_itr, DELETE) != IN)
            throw Exception(ConstNames::Not_Found_msg);
        request_method method = func_delete_map[*command_itr];
        data.erase(command_itr);
        (this->*method)(data);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::signup(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Offline_Mode);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::Signup_Order);
        if(has_permission_to_signup(user_info) == ConstNames::Permissible){
            responding(ConstNames::OK_msg);
        }
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::login(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Offline_Mode);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::Login_Order);
        if(has_permission_to_login(user_info) == ConstNames::Permissible)
            responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::logout(data_t data)
{
    if(data.empty())
    {
        check_access(ConstNames::Online_Mode);
        object_relational->logout_user(*online_users.begin());
        online_users.clear();
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::wallet_post(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::POST_Wallet_Order);
        if(has_permission_to_deposit(user_info) == ConstNames::Permissible)
            responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_reserve(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        Database::User::ReserveInfo reserve_info = fill_reserve_info(data);
        booked_out(reserve_info);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_comment(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        string hotel_id = find_info(ConstNames::Hotel, data);
        string comment = find_info(ConstNames::Comment, data);
        object_relational->post_comment(*online_users.begin(), hotel_id, comment);
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_rating(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        auto rating_info = fill_rating_info(data);
        object_relational->post_rating(rating_info);
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_manual_weights(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        auto manual_weights = fill_manual_weights_info(data);
        object_relational->post_manual_weights(manual_weights, *online_users.begin());
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_filter(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        Database::User::FilterInfo filter_info = fill_filter_info(data);
        set_filters(filter_info);
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_default_price_filter(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        auto activation_mode = fill_default_filter_info(data);
        set_filters(activation_mode);
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::post_sort(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        auto sort_info = fill_sort_info(data);
        set_sort_info(sort_info);
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::wallet_get(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::GET_Wallet_Order);
        check_account(user_info);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::get_reserve(data_t data)
{
    if(data.empty())
    {
        check_access(ConstNames::Online_Mode);
        object_relational->get_reserve(*online_users.begin());
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::get_full_hotel(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        info_t hotel_id = find_info(ConstNames::ID, data);
        get_hotels_info(hotel_id);
    }
    else
        return get_hotels();
}

void Backend::get_hotels()
{
    check_access(ConstNames::Online_Mode);
    get_hotels_info();
}

void Backend::get_comment(data_t data)
{
   if(!data.empty())
   {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        string hotel_id = find_info(ConstNames::Hotel, data);
        object_relational->get_comment(hotel_id);
   }
   else
    throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::get_rating(data_t data)
{
  if(!data.empty())
  {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        string hotel_id = find_info(ConstNames::Hotel, data);
        object_relational->get_rating(hotel_id);
  }
  else
    throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::get_manual_weights(data_t data)
{
    if(data.empty())
    {
        check_access(ConstNames::Online_Mode);
        object_relational->get_manual_weights(*online_users.begin());
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::get_estimated_weights(data_t data)
{
    if(data.empty())
    {
        check_access(ConstNames::Online_Mode);
        object_relational->get_estimated_weights(*online_users.begin());
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::delete_filter(data_t data)
{
    if(data.empty())
    {
        check_access(ConstNames::Online_Mode);
        object_relational->delete_filter(*online_users.begin());
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

void Backend::delete_reserve(data_t data)
{
    if(!data.empty())
    {
        check_question_mark(data);
        check_access(ConstNames::Online_Mode);
        string str_id = find_info(ConstNames::ID, data);
        if(str_id == ConstNames::Empty_Str)
            throw Exception(ConstNames::Bad_Request_msg);
        int id = stoi(str_id);
        object_relational->delete_reserve(*online_users.begin(), id);
        responding(ConstNames::OK_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::has_permission_to_signup(const Database::UserInfo& user_info)
{
    if(object_relational->authentication(user_info, ConstNames::Signup_Order) != ConstNames::Exist){
        online_users.push_back(object_relational->get_specific_user(user_info, ConstNames::Signup_Order));
        return ConstNames::Permissible;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::has_permission_to_login(const Database::UserInfo& user_info)
{
    if(object_relational->authentication(user_info, ConstNames::User_Authentication) == ConstNames::Exist){
        online_users.push_back(object_relational->get_specific_user(user_info, ConstNames::User_Authentication));
        return ConstNames::Permissible;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::has_permission_to_deposit(const Database::UserInfo& user_info)
{
    if(user_info.amount <= ConstNames::Minimum_Deposit_Amount)
        throw Exception(ConstNames::Bad_Request_msg);
    object_relational->deposit_in(user_info, *online_users.begin());
    return ConstNames::Permissible;
}

void Backend::check_account(const Database::UserInfo& user_info)
{
    if(user_info.no_transactions <= ConstNames::Minimum_No_Transactions)
        throw Exception(ConstNames::Bad_Request_msg);
    object_relational->get_account_information(user_info, *online_users.begin());
}

void Backend::get_hotels_info()
{
    object_relational->get_hotels(*online_users.begin());
}

void Backend::get_hotels_info(info_t hotel_id)
{
    object_relational->get_hotels(hotel_id);
}

bool Backend::request_from_online_user(Database::UserInfo user_info)
{
    if(online_users.empty())
        return !ConstNames::Exist;
    if(!online_users.empty())
        return ConstNames::Exist;
    return object_relational->authentication(user_info, ConstNames::Online_User_Check, online_users);
}

Database::UserInfo Backend::fill_user_info(const data_t& data, string mode)
{
    Database::UserInfo user_info = Database::UserInfo();
    if(mode == ConstNames::Signup_Order || mode == ConstNames::Login_Order)
    {
        string email = (find_info(ConstNames::Email, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Email, data) : throw Exception(ConstNames::Bad_Request_msg);
        user_info.email = email;
        string password = (find_info(ConstNames::Password, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Password, data) : throw Exception(ConstNames::Bad_Request_msg);
        user_info.password = hashing(password);
        if(!check_email_validity(user_info.email))
            throw Exception(ConstNames::Bad_Request_msg);
        if(mode == ConstNames::Signup_Order){
            string username = (find_info(ConstNames::Username, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Username, data) : throw Exception(ConstNames::Bad_Request_msg);
            user_info.username = username;
            return user_info;
        }
        user_info.username = ConstNames::Empty_Str;
        return user_info;
    }
    else if(mode == ConstNames::POST_Wallet_Order)
    {
        string amount = (find_info(ConstNames::Amount, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Amount, data) : throw Exception(ConstNames::Bad_Request_msg);
        user_info.amount = stold(amount);
        return user_info;
    }
    else if(mode == ConstNames::GET_Wallet_Order)
    {
        string count = (find_info(ConstNames::Count, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Count, data) : throw Exception(ConstNames::Bad_Request_msg);
        user_info.no_transactions = stoi(count);
    }
    return user_info;
}

Database::User::SortInfo Backend::fill_sort_info(const data_t& data)
{
    auto sort_info = Database::User::SortInfo();
    string property = find_info(ConstNames::Property, data);
    string mode = find_info(ConstNames::Order, data);
    if(!valid_sort_info(property, mode))
        throw Exception(ConstNames::Bad_Request_msg);
    sort_info.property = sort_info.map_sort_property[property];
    sort_info.mode = (mode == ConstNames::Ascending) ? Database::User::SortMode::SM_Ascending : Database::User::SortMode::SM_Descending;
    return sort_info;
}

Database::User::FilterInfo Backend::fill_filter_info(const data_t& data)
{
    Database::User::FilterInfo filter_info = Database::User::FilterInfo();
    string min_star = find_info(ConstNames::Min_Star, data);
    string max_star = find_info(ConstNames::Max_Star, data);
    string min_price = find_info(ConstNames::Min_Price, data);
    string max_price = find_info(ConstNames::Max_Price, data);
    string type = find_info(ConstNames::Type, data);
    string quantity = find_info(ConstNames::Quantity, data);
    string check_in = find_info(ConstNames::Check_In, data);
    string check_out = find_info(ConstNames::Check_Out, data);
    string city = find_info(ConstNames::City, data);
    if(city != ConstNames::Empty_Str)
    {
        filter_info.city = city;
        filter_info.mode = Database::User::FilterMode::City;
    }
    else if(valid_hotel_star(min_star, max_star))
    {
        filter_info.min_star = stoi(min_star);
        filter_info.max_star = stoi(max_star);
        filter_info.mode = Database::User::FilterMode::StarRange;
    }
    else if(valid_price_range(min_price, max_price))
    {
        filter_info.min_price = stold(min_price);
        filter_info.max_price = stold(max_price);
        filter_info.mode = Database::User::FilterMode::AvgPrice;
    }
    else if(valid_advanced_filter(type, quantity, check_in, check_out))
    {
        filter_info.type = filter_info.m_room_type[type];
        filter_info.quantity = stoi(quantity);
        filter_info.check_in = stoi(check_in);
        filter_info.check_out = stoi(check_out);
        filter_info.mode = Database::User::FilterMode::Advanced;
    }
    return filter_info;
}

Database::User::ReserveInfo Backend::fill_reserve_info(const data_t& data)
{
    Database::User::ReserveInfo reserve_info = Database::User::ReserveInfo();
    string hotel_id, quantity, type, check_in, check_out;
    hotel_id = find_info(ConstNames::Hotel, data);
    type = find_info(ConstNames::Type, data);
    quantity = find_info(ConstNames::Quantity, data);
    check_in = find_info(ConstNames::Check_In, data);
    check_out = find_info(ConstNames::Check_Out, data);
    if(hotel_id != ConstNames::Empty_Str && valid_advanced_filter(type, quantity, check_in, check_out))
    {
        reserve_info.hotel_id = hotel_id;
        reserve_info.check_in = stoi(check_in);
        reserve_info.check_out = stoi(check_out);
        reserve_info.quantity = stoi(quantity);
        reserve_info.type = reserve_info.m_room_type[type];
        return reserve_info;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Database::Hotel::RatingInfo Backend::fill_rating_info(const data_t& data, const string mode)
{
    Database::Hotel::RatingInfo rating_info;
    string hotel_id = ConstNames::Dump_str_Number, location, cleanliness, staff, facilities, value_for_money, overall_rating = ConstNames::Dump_str_Number;
    location = find_info(ConstNames::Location, data) != ConstNames::Empty_Str ? find_info(ConstNames::Location, data) : throw Exception(ConstNames::Bad_Request_msg);
    cleanliness = find_info(ConstNames::Cleanliness, data) != ConstNames::Empty_Str ? find_info(ConstNames::Cleanliness, data) : throw Exception(ConstNames::Bad_Request_msg);
    staff = find_info(ConstNames::Staff, data) != ConstNames::Empty_Str ? find_info(ConstNames::Staff, data) : throw Exception(ConstNames::Bad_Request_msg);
    facilities = find_info(ConstNames::Facilities, data) != ConstNames::Empty_Str ? find_info(ConstNames::Facilities, data) : throw Exception(ConstNames::Bad_Request_msg);
    value_for_money = find_info(ConstNames::Value_For_Money, data) != ConstNames::Empty_Str ? find_info(ConstNames::Value_For_Money, data) : throw Exception(ConstNames::Bad_Request_msg);
    if(mode == ConstNames::Rating_Order){
        hotel_id = (find_info(ConstNames::Hotel, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Hotel, data) : throw Exception(ConstNames::Bad_Request_msg);
        overall_rating = (find_info(ConstNames::Overall_Rating, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Overall_Rating, data) : throw Exception(ConstNames::Bad_Request_msg);
    }
    if(valid_rating_info(hotel_id, location, cleanliness, staff, facilities, value_for_money, overall_rating))
    {
        rating_info.hotel_id = hotel_id;
        rating_info.location = stold(location);
        rating_info.cleanliness = stold(cleanliness);
        rating_info.staff = stold(staff);
        rating_info.facilities = stold(facilities);
        rating_info.value_for_money = stold(value_for_money);
        rating_info.overall_rating = stold(overall_rating);
        rating_info.user = *online_users.begin();
        return rating_info;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Database::User::Weights Backend::fill_manual_weights_info(const data_t& data)
{
    auto weights = Database::Hotel::RatingInfo();
    auto activity_status = (find_info(ConstNames::Active, data) == ConstNames::True) ? ConstNames::Active_Mode : (find_info(ConstNames::Active, data) == ConstNames::False) ?  ConstNames::Inactive_Mode : throw Exception(ConstNames::Bad_Request_msg);
    if(activity_status)
        weights = fill_rating_info(data, ConstNames::Manual_Weights_Order);
    return Database::User::Weights(weights, activity_status);
}

bool Backend::valid_sort_info(const std::string& property, const std::string mode)
{
    auto temp_sort = Database::User::SortInfo();
    auto property_i = temp_sort.map_sort_property.find(property);
    if(property_i == temp_sort.map_sort_property.end())
        return false;
    if(mode != ConstNames::Ascending && mode != ConstNames::Descending)
        return false;
    return true;
}

bool Backend::fill_default_filter_info(const data_t& data)
{
    if(!data.empty())
    {
        string activation_mode = find_info(ConstNames::Active, data);
        return (activation_mode == ConstNames::True) ? true : (activation_mode == ConstNames::False) ? false : throw Exception(ConstNames::Bad_Request_msg);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::valid_rating_info(std::string hotel_id, std::string location, std::string cleanliness, std::string staff, std::string facilities, std::string value_for_money, std::string overall_rating)
{
    if(hotel_id != ConstNames::Empty_Str && location != ConstNames::Empty_Str && cleanliness != ConstNames::Empty_Str && staff != ConstNames::Empty_Str && facilities != ConstNames::Empty_Str && value_for_money != ConstNames::Empty_Str && overall_rating != ConstNames::Empty_Str)
    {
        vector<long double> ratings = {stold(location), stold(cleanliness), stold(staff), stold(facilities), stold(value_for_money), stold(overall_rating)};
        for(long double rate : ratings)
        {
            if(!in_range(rate, ConstNames::Min_Rate, ConstNames::Max_Rate))
                throw Exception(ConstNames::Bad_Request_msg);
        }
        return true;
    }
    return false;
}

bool Backend::valid_hotel_star(string min_star, string max_star)
{
    if(min_star != ConstNames::Empty_Str && max_star != ConstNames::Empty_Str)
    {
        int min = stoi(min_star), max = stoi(max_star);
        if(min >= ConstNames::Minimum_Hotel_Star && max >= min && max <= ConstNames::Maximum_Hotel_Star)
            return true;
        else
            throw Exception(ConstNames::Bad_Request_msg);
    }
    return false;
}

bool Backend::valid_price_range(string min_price, string max_price)
{
    if(min_price != ConstNames::Empty_Str && max_price != ConstNames::Empty_Str)
    {
        int min = stold(min_price), max = stold(max_price);
        if(max >= min && min >= 0 && max >= 0)
            return true;
        else
            throw Exception(ConstNames::Bad_Request_msg);
    }
    return false;
}

bool Backend::valid_advanced_filter(string type, string quantity, string check_in, string check_out)
{
    if(type != ConstNames::Empty_Str && quantity != ConstNames::Empty_Str && check_in != ConstNames::Empty_Str && check_out != ConstNames::Empty_Str)
    {
        int q = stoi(quantity), in = stoi(check_in), out = stoi(check_out);
        if(valid_advanced_filter_type(type) && q > ConstNames::No_Room && in >= ConstNames::Minimum_Hotel_Reserve && in <= out && out <= ConstNames::Maximum_Hotel_Reserve)
            return true;
        else
            throw Exception(ConstNames::Bad_Request_msg);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::valid_advanced_filter_type(string type)
{
    if(type == ConstNames::StandardRoom || type == ConstNames::DeluxeRoom || type == ConstNames::LuxuryRoom || type == ConstNames::PremiumRoom)
        return true;
    return false;
}

Content Backend::find_info(const string& mode, const data_t& data)
{
    auto iterator = find(data.begin(), data.end(), mode);
    if(info_exist(iterator, iterator + 1, data.end()) != ConstNames::Exist)
        return ConstNames::Empty_Str;
    iterator++;
    return (*iterator);
}

bool Backend::in_the_command(const std::string& command, enum RequestType type)
{
    if(type == POST && in_the_post_command(command) == IN)
        return IN;
    else if(type == GET && in_the_get_command(command) == IN)
        return IN;
    else if(type == DELETE && in_the_delete_command(command) == IN)
        return IN;
    return !IN;
}

bool Backend::in_the_post_command(const std::string& command)
{
    auto command_itr = func_post_map.find(command);
    if(command_itr == func_post_map.end())
        return !IN;
    return IN;
}

bool Backend::in_the_get_command(const std::string& command)
{
    auto command_itr = func_get_map.find(command);
    if(command_itr == func_get_map.end())
        return !IN;
    return IN;
}

bool Backend::in_the_delete_command(const std::string& command)
{
    auto command_itr = func_delete_map.find(command);
    if(command_itr == func_delete_map.end())
        return !IN;
    return IN;
}

template<typename iterator>
bool Backend::info_exist(iterator it, iterator next, iterator end)
{
    if(it != end && next != end)
        return ConstNames::Exist;
    return !ConstNames::Exist;
}

template<class T>
hash_t Backend::hashing(T raw)
{
    hash<T> hash_password;
    return hash_password(raw);
}

bool Backend::check_email_validity(const string& email)
{
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email,pattern);
}

template <class T>
bool Backend::in_range(T value, T min, T max)
{
    if(value >= min && value <= max)
        return true;
    return false;
}

void Backend::set_filters(const Database::User::FilterInfo& filter_info)
{
    object_relational->set_filter(filter_info, *online_users.begin());
}

void Backend::set_filters(const bool activation_mode)
{
    object_relational->set_filter(activation_mode, *online_users.begin());
}

void Backend::set_sort_info(const Database::User::SortInfo& sort_info)
{
    object_relational->set_sort_info(sort_info, *online_users.begin());
}

void Backend::booked_out(Database::User::ReserveInfo& reserve_info)
{
    object_relational->booked_out(reserve_info, *online_users.begin());
}

void Backend::construct_maps()
{
    func_method_map.insert(make_pair(ConstNames::POST_METHOD, &Backend::post_request));
    func_method_map.insert(make_pair(ConstNames::GET_METHOD, &Backend::get_request));
    func_method_map.insert(make_pair(ConstNames::DELETE_METHOD, &Backend::delete_request));
    func_post_map.insert(make_pair(ConstNames::Signup_Order, &Backend::signup));
    func_post_map.insert(make_pair(ConstNames::Login_Order, &Backend::login));
    func_post_map.insert(make_pair(ConstNames::Logout_Order, &Backend::logout));
    func_post_map.insert(make_pair(ConstNames::Wallet_Order, &Backend::wallet_post));
    func_post_map.insert(make_pair(ConstNames::Filter_Order, &Backend::post_filter));
    func_post_map.insert(make_pair(ConstNames::Reserve_Order, &Backend::post_reserve));
    func_post_map.insert(make_pair(ConstNames::Comment_Order, &Backend::post_comment));
    func_post_map.insert(make_pair(ConstNames::Rating_Order, &Backend::post_rating));
    func_post_map.insert(make_pair(ConstNames::Default_Price_Filter_Order, &Backend::post_default_price_filter));
    func_post_map.insert(make_pair(ConstNames::Sort_Order, &Backend::post_sort));
    func_post_map.insert(make_pair(ConstNames::Manual_Weights_Order, &Backend::post_manual_weights));
    func_get_map.insert(make_pair(ConstNames::Wallet_Order, &Backend::wallet_get));
    func_get_map.insert(make_pair(ConstNames::Get_Hotels_Order, &Backend::get_full_hotel));
    func_get_map.insert(make_pair(ConstNames::Reserve_Order, &Backend::get_reserve));
    func_get_map.insert(make_pair(ConstNames::Comment_Order, &Backend::get_comment));
    func_get_map.insert(make_pair(ConstNames::Rating_Order, &Backend::get_rating));
    func_get_map.insert(make_pair(ConstNames::Manual_Weights_Order, &Backend::get_manual_weights));
    func_get_map.insert(make_pair(ConstNames::Estimated_Weights_Order, &Backend::get_estimated_weights));
    func_delete_map.insert(make_pair(ConstNames::Filter_Order, &Backend::delete_filter));
    func_delete_map.insert(make_pair(ConstNames::Reserve_Order, &Backend::delete_reserve));
}

void Backend::check_question_mark(data_t& data)
{
    auto command_itr = data.begin();
    if(*command_itr != ConstNames::Question_Mark)
        throw Exception(ConstNames::Bad_Request_msg);
    data.erase(command_itr);
}

void Backend::check_access(const string& mode)
{
    if(mode == ConstNames::Online_Mode)
    {
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
    }
    else
    {
        if(request_from_online_user() == ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
    }
}

template<class T>
void API::responding(T response)
{
    if(instance->result != nullptr)
        delete instance->result;
    instance->result = new Result(response);
}

void Backend::responding(string message)
{
    auto api = API::get_instance();
    api->responding(message);
}

Database::User* Backend::get_curr_user() { return *online_users.begin(); }

Database::User::Weights Backend::calculate_weights(Database::User* user, Database::Hotel::v_rating ratings)
{
    auto ai = new AI(user);
    return ai->get_calculated_weights(ratings);
}

long double Backend::calculate_weighted_average(Database::User::Weights weights, Database::Hotel::v_rating::iterator rating_itr)
{
    auto v_weights = instance->get_vector_of_weights(weights);
    auto sum = (*rating_itr)->get_sum_weighted(v_weights);
    auto weighted_sum = accumulate(v_weights.begin(), v_weights.end(), 0);
    return sum / weighted_sum;
}

long double Backend::calculate_weighted_average(v_double weights, Database::Hotel::v_rating::iterator rating_itr)
{
    auto sum = (*rating_itr)->get_sum_weighted(weights);
    auto weighted_sum = accumulate(weights.begin(), weights.end(), 0.0);
    return sum / weighted_sum;
}

v_double Backend::get_vector_of_weights(Database::User::Weights weights)
{
    auto temp = v_double();
    for(int count = 0; count < ConstNames::Maximum_Weights_Number; count++)
        temp.push_back(weights[count]);
    return temp;
}



