#ifndef _DATABASE_H
#define _DATABASE_H

#include <list>
#include <string>
#include <vector>

typedef std::string info_t;
typedef std::vector<info_t> v_info;
typedef int num_t;
typedef float price_t;
typedef float coordinate_t;

class Database
{
public:
    struct HotelInfo
    {
        info_t unique_id;
        info_t property_name;
        num_t hotel_rating;
        info_t hotel_overview;
        info_t property_amenities;
        info_t city;
        coordinate_t latitude;
        coordinate_t longitude;
        info_t img_url;
        num_t no_standard_rooms;
        num_t no_deluxe_rooms;
        num_t no_luxury_rooms;
        num_t no_premium_rooms;
        price_t standard_room_price;
        price_t deluxe_room_price;
        price_t luxury_room_price;
        price_t premium_room_price;
        HotelInfo(info_t u_id, info_t prop_name, num_t hotel_r, info_t hotel_ov, info_t prop_amenities,
                  info_t i_city, float latude, float lotude, info_t i_url, num_t n_s_r, num_t n_d_r,
                  num_t n_l_r, num_t n_p_r, price_t s_p, price_t d_p, price_t l_p, price_t p_p);
    };

    struct UserInfo
    {
        info_t email;
        info_t username;
        long long password;
        float amount;
        int no_transactions;
        UserInfo(info_t i_email = "", info_t i_username = "", long long i_password = 0, float i_amount = 0.0f)
        {
            email = i_email;
            username = i_username;
            password = i_password;
            amount = i_amount;
            no_transactions = 0;
        }
    };

public:
    class User;
    class Hotel;
    class Comment;
    typedef std::list<User*> l_users;
    typedef std::list<Hotel*> l_hotels;
    typedef std::list<Comment*> l_comments;

public:
    Database();
    ~Database();

    void hotel_setup(const HotelInfo& hotel_info);
    void user_setup(const UserInfo& user_info);
    void increase_user_amount(const UserInfo& user_info, User* user);
    void post_comment(User* user, std::string hotel_id, std::string comment);

    info_t print_account_information(const UserInfo& user_info, User* user);
    info_t get_hotels(User* user);
    info_t get_hotels(info_t hotel_id);
    info_t booked_out(User* user, Hotel* hotel, int quantity, int check_in, int check_out, int type);
    info_t get_reserve(User* user);

    User* query_in_users(UserInfo user_info);
    User* query_in_users(UserInfo user_info, const l_users& users);

    Hotel* query_in_hotels(info_t hotel_id);

    l_comments get_comment(std::string hotel_id);

private:
    bool check_info(User* current_user, UserInfo user_info);

private:
    l_users users;
    l_hotels hotels;
    l_comments comments;

};
#endif
