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
        v_info property_amenities;
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
        HotelInfo(info_t u_id, info_t prop_name, num_t hotel_r, info_t hotel_ov, v_info prop_amenities,
                  info_t i_city, float latude, float lotude, info_t i_url, num_t n_s_r, num_t n_d_r,
                  num_t n_l_r, num_t n_p_r, price_t s_p, price_t d_p, price_t l_p, price_t p_p);
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


private:
    l_users users;
    l_hotels hotels;
    l_comments comments;

};
#endif
