#ifndef _BACKEND_H
#define _BACKEND_H

#include <vector>
#include <string>
#include <map>
#include "../Models/Database.hpp"

#define IN true

typedef std::string Content;
typedef std::string request_t;
typedef long long hash_t;

class ObjectRelational;

class Backend
{
public:
    typedef std::vector<std::string> data_t;
    typedef Content (Backend::*request_method) (data_t);
    typedef std::map<request_t, request_method> map_request;
    typedef std::map<request_t, request_method> m_post_request;
    typedef std::map<request_t, request_method> m_get_request;
    enum CommandOrder
    {
        Method_Type,
        Command_Type,
        Question_Mark
    };
    enum RequestType
    {
        POST,
        GET,
        DELETE
    };

public:
    ~Backend();
    static Backend* get_instance();
    static void release();
    data_t parse_content(Content content, char separator);
    Content command_processor(data_t command_data);

private:
    Backend();
    Content post_request(data_t data);
    Content get_request(data_t data);
    Content signup(data_t data);
    Content login(data_t data);
    Content logout(data_t data);
    Content wallet_post(data_t data);
    Content wallet_get(data_t data);
    Content get_full_hotel(data_t data);
    Content get_hotels();
    bool in_the_command(const std::string& command, enum RequestType type);
    bool in_the_post_command(const std::string& command);
    bool in_the_get_command(const std::string& command);
    Database::UserInfo fill_user_info(const data_t& data, const std::string mode);

    template<typename iterator>
    bool info_exist(iterator it, iterator next, iterator end);

    bool check_email_validity(const std::string& email);

    template<class T>
    hash_t hash_password(T raw_password);

    Content find_info(const std::string& mode, const data_t& data);
    bool has_permission_to_signup(const Database::UserInfo& user_info);
    bool has_permission_to_login(const Database::UserInfo& user_info);
    bool has_permission_to_deposit(const Database::UserInfo& user_info);
    Content check_account(const Database::UserInfo& user_info);
    Content get_hotels_info();
    Content get_hotels_info(info_t hotel_id);
    bool request_from_online_user(Database::UserInfo user_info = Database::UserInfo());

private:
    static Backend* instance;
    ObjectRelational* object_relational;
    Database::l_users online_users;
    map_request func_method_map;
    m_post_request func_post_map;
    m_get_request func_get_map;
};

#endif
