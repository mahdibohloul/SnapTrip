#ifndef _BACKEND_H
#define _BACKEND_H

#include <vector>
#include <string>
#include <map>
#include "../Models/User/User.hpp"

#define IN true

typedef std::string Content;
typedef std::string request_t;
typedef long long hash_t;

class ObjectRelational;

class Backend
{
public:
    typedef std::vector<std::string> data_t;
    typedef void (Backend::*request_method) (data_t);
    typedef std::map<request_t, request_method> map_request;
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
    void command_processor(data_t command_data);

    Database::User* get_curr_user();

private:
    Backend();
    void post_request(data_t data);
    void get_request(data_t data);
    void delete_request(data_t data);
    void signup(data_t data);
    void login(data_t data);
    void logout(data_t data);
    void wallet_post(data_t data);
    void post_filter(data_t data);
    void post_default_price_filter(data_t data);
    void post_reserve(data_t data);
    void post_comment(data_t data);
    void post_rating(data_t data);
    void wallet_get(data_t data);
    void get_full_hotel(data_t data);
    void get_hotels();
    void get_reserve(data_t data);
    void get_comment(data_t data);
    void get_rating(data_t data);
    void delete_filter(data_t data);
    void delete_reserve(data_t data);
    void booked_out(Database::User::ReserveInfo& reserve_info);
    void check_account(const Database::UserInfo& user_info);
    void get_hotels_info();
    void get_hotels_info(info_t hotel_id);
    void set_filters(const Database::User::FilterInfo& filter_info);
    void set_filters(const bool activation_mode);
    void construct_maps();
    void check_question_mark(data_t& data);
    void responding(std::string message);

    bool in_the_command(const std::string& command, enum RequestType type);
    bool in_the_post_command(const std::string& command);
    bool in_the_get_command(const std::string& command);
    bool in_the_delete_command(const std::string& command);
    template<typename iterator> bool info_exist(iterator it, iterator next, iterator end);
    bool check_email_validity(const std::string& email);
    bool has_permission_to_signup(const Database::UserInfo& user_info);
    bool has_permission_to_login(const Database::UserInfo& user_info);
    bool has_permission_to_deposit(const Database::UserInfo& user_info);
    bool request_from_online_user(Database::UserInfo user_info = Database::UserInfo());
    bool valid_hotel_star(std::string min_star, std::string max_star);
    bool valid_price_range(std::string min_price, std::string max_price);
    bool valid_advanced_filter(std::string type, std::string quantity, std::string check_in, std::string check_out);
    bool valid_advanced_filter_type(std::string type);
    bool valid_rating_info(std::string hotel_id, std::string location, std::string cleanliness, std::string staff, std::string facilities, std::string value_for_money, std::string overall_rating);
    bool fill_default_filter_info(const data_t& data);
    template <class T> bool in_range(T value, T min, T max);

    Content find_info(const std::string& mode, const data_t& data);

    Database::UserInfo fill_user_info(const data_t& data, const std::string mode);
    Database::User::FilterInfo fill_filter_info(const data_t& data);
    Database::User::ReserveInfo fill_reserve_info(const data_t& data);
    Database::Hotel::RatingInfo fill_rating_info(const data_t& data);

    template<class T>
    hash_t hashing(T raw);

private:
    static Backend* instance;
    ObjectRelational* object_relational;
    Database::l_users online_users;
    map_request func_method_map;
    map_request func_post_map;
    map_request func_get_map;
    map_request func_delete_map;
};

#endif
