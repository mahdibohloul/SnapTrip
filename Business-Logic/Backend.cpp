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
    func_method_map.insert(make_pair(ConstNames::Signup_Order, &Backend::signup));
    func_method_map.insert(make_pair(ConstNames::Login_Order, &Backend::login));
    func_method_map.insert(make_pair(ConstNames::Logout_Order, &Backend::logout));
    post_method.insert(make_pair(ConstNames::Signup_Order, Signup));
    post_method.insert(make_pair(ConstNames::Login_Order, Login));
    post_method.insert(make_pair(ConstNames::Logout_Order, Logout));
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
        request_method method = func_method_map[*command_itr];
        data.erase(command_itr);
        return (this->*method)(data);
    }
    else
        throw Exception(ConstNames::Bad_Request_msg);
}

Content Backend::get_request(data_t data)
{

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
    if(!online_users.empty())
    {
        online_users.clear();
        return ConstNames::OK_msg;
    }
    else
        throw Exception(ConstNames::Permission_Denied_msg);
}

bool Backend::has_permission_to_signup(Database::UserInfo user_info)
{
    if(request_from_online_user(user_info) == ConstNames::Exist)
        throw Exception(ConstNames::Permission_Denied_msg);
    if(object_relational->authentication(user_info, ConstNames::Signup_Order) != ConstNames::Exist){
        online_users.push_back(object_relational->get_specific_user(user_info, ConstNames::Signup_Order));
        return ConstNames::Permissible;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::has_permission_to_login(Database::UserInfo user_info)
{
    if(request_from_online_user(user_info) == ConstNames::Exist)
        throw Exception(ConstNames::Permission_Denied_msg);
    if(object_relational->authentication(user_info, ConstNames::User_Authentication) == ConstNames::Exist){
        online_users.push_back(object_relational->get_specific_user(user_info, ConstNames::User_Authentication));
        return ConstNames::Permissible;
    }
    throw Exception(ConstNames::Bad_Request_msg);
}

bool Backend::request_from_online_user(Database::UserInfo user_info)
{
    return object_relational->authentication(user_info, ConstNames::Online_User_Check, online_users);
}

Database::UserInfo Backend::fill_user_info(const data_t& data, string mode)
{
    Database::UserInfo user_info;
    user_info.email = find_info(ConstNames::Email, data);
    user_info.password = hash_password(find_info(ConstNames::Password, data));
    if(!check_email_validity(user_info.email))
        throw Exception(ConstNames::Bad_Request_msg);
    if(mode == ConstNames::Signup_Order)
    {
        user_info.username = find_info(ConstNames::Username, data);
        return user_info;
    }
    user_info.username = ConstNames::Empty_Str;
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
    return !IN;
}

bool Backend::in_the_post_command(const std::string& command)
{
    auto command_itr = post_method.find(command);
    if(command_itr == post_method.end())
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
