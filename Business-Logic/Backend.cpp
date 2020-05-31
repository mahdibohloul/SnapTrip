#include "Backend.hpp"
#include "../ORM/ObjectRelational.hpp"
#include "../ConstNames.hpp"
#include "../Exception/Exception.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <regex>
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

Backend::~Backend() {}


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

Content Backend::command_processor(data_t command_data)
{
    if(!command_data.empty())
    {
        object_relational = ObjectRelational::get_instance();
        auto command_itr = command_data.begin();
        if(*command_itr != ConstNames::POST_METHOD && *command_itr != ConstNames::GET_METHOD && *command_itr != ConstNames::DELETE_METHOD)
            throw Exception(ConstNames::Bad_Request_msg);
        request_method method = func_method_map[*command_itr];
        command_data.erase(command_itr);
        return (this->*method)(command_data);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::post_request(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(in_the_command(*command_itr, POST) != IN)
            throw Exception(ConstNames::Not_Found_msg);
        request_method method = func_post_map[*command_itr];
        data.erase(command_itr);
        return (this->*method)(data);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::get_request(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(in_the_command(*command_itr, GET) != IN)
            throw Exception(ConstNames::Not_Found_msg);
        request_method method = func_get_map[*command_itr];
        data.erase(command_itr);
        return (this->*method)(data);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::delete_request(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(in_the_command(*command_itr, DELETE) != IN)
            throw Exception(ConstNames::Not_Found_msg);
        request_method method = func_delete_map[*command_itr];
        data.erase(command_itr);
        return (this->*method)(data);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::signup(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::Signup_Order);
        if(has_permission_to_signup(user_info) == ConstNames::Permissible)
            return ConstNames::OK_msg;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::login(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::Login_Order);
        if(has_permission_to_login(user_info) == ConstNames::Permissible)
            return ConstNames::OK_msg;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::logout(data_t data)
{
    if(data.empty())
    {
        if(!online_users.empty())
        {
            object_relational->logout_user(*online_users.begin());
            online_users.clear();
            return ConstNames::OK_msg;
        }
        else
            throw Exception(ConstNames::Permission_Denied_msg);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::wallet_post(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::POST_Wallet_Order);
        if(has_permission_to_deposit(user_info) == ConstNames::Permissible)
            return ConstNames::OK_msg;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::post_reserve(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        data.erase(command_itr);
        Database::User::ReserveInfo reserve_info = fill_reserve_info(data);
        return booked_out(reserve_info);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::post_comment(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        data.erase(command_itr);
        string hotel_id = find_info(ConstNames::Hotel, data);
        string comment = find_info(ConstNames::Comment, data);
        object_relational->post_comment(*online_users.begin(), hotel_id, comment);
        return ConstNames::OK_msg;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::post_rating(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        data.erase(command_itr);
        auto rating_info = fill_rating_info(data);
        object_relational->post_rating(rating_info);
        return ConstNames::OK_msg;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::post_filter(data_t data)
{
    if(!data.empty())
    {
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        Database::User::FilterInfo filter_info = fill_filter_info(data);
        set_filters(filter_info);
        return ConstNames::OK_msg;
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::wallet_get(data_t data)
{
    if(!data.empty())
    {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        Database::UserInfo user_info = fill_user_info(data, ConstNames::GET_Wallet_Order);
        return check_account(user_info);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::get_reserve(data_t data)
{
    if(data.empty())
    {
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        return object_relational->get_reserve(*online_users.begin());
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::get_full_hotel(data_t data)
{
    if(!data.empty())
    {
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        info_t hotel_id = find_info(ConstNames::ID, data);
        return get_hotels_info(hotel_id);
    }
    else
        return get_hotels();
}

Content Backend::get_hotels()
{
    if(request_from_online_user() != ConstNames::Exist)
        throw Exception(ConstNames::Permission_Denied_msg);
    return get_hotels_info();
}

Content Backend::get_comment(data_t data)
{
   if(!data.empty())
   {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        string hotel_id = find_info(ConstNames::Hotel, data);
        return object_relational->get_comment(hotel_id);
   }
   throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::get_rating(data_t data)
{
  if(!data.empty())
  {
        auto command_itr = data.begin();
        if(*command_itr != ConstNames::Question_Mark)
            throw Exception(ConstNames::Bad_Request_msg);
        data.erase(command_itr);
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        string hotel_id = find_info(ConstNames::Hotel, data);
        return object_relational->get_rating(hotel_id);
  }
  else
    throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::delete_filter(data_t data)
{
    if(data.empty())
    {
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        object_relational->delete_filter(*online_users.begin());
        return ConstNames::OK_msg;
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::delete_reserve(data_t data)
{
    if(!data.empty())
    {
        if(request_from_online_user() != ConstNames::Exist)
            throw Exception(ConstNames::Permission_Denied_msg);
        string str_id = find_info(ConstNames::ID, data);
        if(str_id == ConstNames::Empty_Str)
            throw Exception(ConstNames::Bad_Request_msg);
        int id = stoi(str_id);
        return object_relational->delete_reserve(*online_users.begin(), id);
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::has_permission_to_signup(const Database::UserInfo& user_info)
{
    if(request_from_online_user(user_info) == ConstNames::Exist)
        throw Exception(ConstNames::Permission_Denied_msg);
    if(object_relational->authentication(user_info, ConstNames::Signup_Order) != ConstNames::Exist){
        online_users.push_back(object_relational->get_specific_user(user_info, ConstNames::Signup_Order));
        return ConstNames::Permissible;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::has_permission_to_login(const Database::UserInfo& user_info)
{
    if(request_from_online_user(user_info) == ConstNames::Exist)
        throw Exception(ConstNames::Permission_Denied_msg);
    if(object_relational->authentication(user_info, ConstNames::User_Authentication) == ConstNames::Exist){
        online_users.push_back(object_relational->get_specific_user(user_info, ConstNames::User_Authentication));
        return ConstNames::Permissible;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::has_permission_to_deposit(const Database::UserInfo& user_info)
{
    if(request_from_online_user(user_info) != ConstNames::Exist)
        throw Exception(ConstNames::Permission_Denied_msg);
    if(user_info.amount <= ConstNames::Minimum_Deposit_Amount)
        throw Exception(ConstNames::Bad_Request_msg);
    object_relational->deposit_in(user_info, *online_users.begin());
    return ConstNames::Permissible;
}

Content Backend::check_account(const Database::UserInfo& user_info)
{
    if(request_from_online_user(user_info) != ConstNames::Exist)
        throw Exception(ConstNames::Permission_Denied_msg);
    if(user_info.no_transactions <= ConstNames::Minimum_No_Transactions)
        throw Exception(ConstNames::Bad_Request_msg);
    return object_relational->print_account_information(user_info, *online_users.begin());
}

Content Backend::get_hotels_info()
{
    return object_relational->get_hotels(*online_users.begin());
}

Content Backend::get_hotels_info(info_t hotel_id)
{
    return object_relational->get_hotels(hotel_id);
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
        user_info.amount = stof(amount);
        return user_info;
    }
    else if(mode == ConstNames::GET_Wallet_Order)
    {
        string count = (find_info(ConstNames::Count, data) != ConstNames::Empty_Str) ? find_info(ConstNames::Count, data) : throw Exception(ConstNames::Bad_Request_msg);
        user_info.no_transactions = stoi(count);
    }
    return user_info;
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
        filter_info.min_price = stof(min_price);
        filter_info.max_price = stof(max_price);
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

Database::Hotel::RatingInfo Backend::fill_rating_info(const data_t& data)
{
    Database::Hotel::RatingInfo rating_info;
    string hotel_id, location, cleanliness, staff, facilities, value_for_money, overall_rating;
    hotel_id = find_info(ConstNames::Hotel, data);
    location = find_info(ConstNames::Location, data);
    cleanliness = find_info(ConstNames::Cleanliness, data);
    staff = find_info(ConstNames::Staff, data);
    facilities = find_info(ConstNames::Facilities, data);
    value_for_money = find_info(ConstNames::Value_For_Money, data);
    overall_rating = find_info(ConstNames::Overall_Rating, data);
    if(valid_rating_info(hotel_id, location, cleanliness, staff, facilities, value_for_money, overall_rating))
    {
        rating_info.hotel_id = hotel_id;
        rating_info.location = stof(location);
        rating_info.cleanliness = stof(cleanliness);
        rating_info.staff = stof(staff);
        rating_info.facilities = stof(facilities);
        rating_info.value_for_money = stof(value_for_money);
        rating_info.overall_rating = stof(overall_rating);
        rating_info.user = *online_users.begin();
        return rating_info;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::valid_rating_info(std::string hotel_id, std::string location, std::string cleanliness, std::string staff, std::string facilities, std::string value_for_money, std::string overall_rating)
{
    if(hotel_id != ConstNames::Empty_Str && location != ConstNames::Empty_Str && cleanliness != ConstNames::Empty_Str && staff != ConstNames::Empty_Str && facilities != ConstNames::Empty_Str && value_for_money != ConstNames::Empty_Str && overall_rating != ConstNames::Empty_Str)
    {
        vector<float> ratings = {stof(location), stof(cleanliness), stof(staff), stof(facilities), stof(value_for_money), stof(overall_rating)};
        for(float rate : ratings)
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
        int min = stof(min_price), max = stof(max_price);
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

Content Backend::booked_out(Database::User::ReserveInfo& reserve_info)
{
    return object_relational->booked_out(reserve_info, *online_users.begin());
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
    func_get_map.insert(make_pair(ConstNames::Wallet_Order, &Backend::wallet_get));
    func_get_map.insert(make_pair(ConstNames::Get_Hotels_Order, &Backend::get_full_hotel));
    func_get_map.insert(make_pair(ConstNames::Reserve_Order, &Backend::get_reserve));
    func_get_map.insert(make_pair(ConstNames::Comment_Order, &Backend::get_comment));
    func_get_map.insert(make_pair(ConstNames::Rating_Order, &Backend::get_rating));
    func_delete_map.insert(make_pair(ConstNames::Filter_Order, &Backend::delete_filter));
    func_delete_map.insert(make_pair(ConstNames::Reserve_Order, &Backend::delete_reserve));
}
