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
    func_method_map.insert(make_pair(ConstNames::POST_METHOD, &Backend::post_request));
    func_method_map.insert(make_pair(ConstNames::GET_METHOD, &Backend::get_request));
    func_post_map.insert(make_pair(ConstNames::Signup_Order, &Backend::signup));
    func_post_map.insert(make_pair(ConstNames::Login_Order, &Backend::login));
    func_post_map.insert(make_pair(ConstNames::Logout_Order, &Backend::logout));
    func_post_map.insert(make_pair(ConstNames::Wallet_Order, &Backend::wallet_post));
    func_get_map.insert(make_pair(ConstNames::Wallet_Order, &Backend::wallet_get));
    func_get_map.insert(make_pair(ConstNames::Get_Hotels_Order, &Backend::get_full_hotel));
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
        if(*command_itr != ConstNames::POST_METHOD && *command_itr != ConstNames::GET_METHOD)
            throw Exception(ConstNames::Bad_Request_msg);
        request_method method = func_method_map[*command_itr];
        command_data.erase(command_itr);
        return (this->*method)(command_data);
    }
    else{
        throw Exception(ConstNames::Bad_Request_msg);
    }
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
    else
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
    else
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
    else
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
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::logout(data_t data)
{
    if(data.empty())
    {
        if(!online_users.empty())
        {
            online_users.clear();
            return ConstNames::OK_msg;
        }
        else
            throw Exception(ConstNames::Permission_Denied_msg);
    }
    else
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
        user_info.email = find_info(ConstNames::Email, data);
        user_info.password = hash_password(find_info(ConstNames::Password, data));
        if(!check_email_validity(user_info.email))
            throw Exception(ConstNames::Bad_Request_msg);
        if(mode == ConstNames::Signup_Order){
            user_info.username = find_info(ConstNames::Username, data);
            return user_info;
        }
        user_info.username = ConstNames::Empty_Str;
        return user_info;
    }
    else if(mode == ConstNames::POST_Wallet_Order)
    {
        user_info.amount = stof(find_info(ConstNames::Amount, data));
        return user_info;
    }
    else if(mode == ConstNames::GET_Wallet_Order)
    {
        user_info.no_transactions = stoi(find_info(ConstNames::Count, data));
    }
    return user_info;
}

Content Backend::find_info(const string& mode, const data_t& data)
{
    auto iterator = find(data.begin(), data.end(), mode);
    if(info_exist(iterator, iterator + 1, data.end()) != ConstNames::Exist)
        throw Exception(ConstNames::Bad_Request_msg);
    iterator++;
    return (*iterator);
}

bool Backend::in_the_command(const std::string& command, enum RequestType type)
{
    if(type == POST && in_the_post_command(command) == IN)
        return IN;
    else if(type == GET && in_the_get_command(command) == IN)
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

template<typename iterator>
bool Backend::info_exist(iterator it, iterator next, iterator end)
{
    if(it != end && next != end)
        return ConstNames::Exist;
    return !ConstNames::Exist;
}

template<class T>
hash_t Backend::hash_password(T password)
{
    hash<T> hash_password;
    return hash_password(password);
}

bool Backend::check_email_validity(const string& email)
{
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email,pattern);
}
