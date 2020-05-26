#include "Database.hpp"
#include "Hotel.hpp"
#include <iostream>
using namespace std;

Database::HotelInfo::HotelInfo(info_t u_id, info_t prop_name, num_t hotel_r, info_t hotel_ov, v_info prop_amenities,
                  info_t i_city, float latude, float lotude, info_t i_url, num_t n_s_r, num_t n_d_r,
                  num_t n_l_r, num_t n_p_r, price_t s_p, price_t d_p, price_t l_p, price_t p_p)
{
    unique_id = u_id;
    property_name = prop_name;
    hotel_rating = hotel_r;
    hotel_overview = hotel_ov;
    property_amenities = prop_amenities;
    city = i_city;
    latitude = latude;
    longitude = lotude;
    img_url = i_url;
    no_standard_rooms = n_s_r;
    no_deluxe_rooms = n_d_r;
    no_luxury_rooms = n_l_r;
    no_premium_rooms = n_p_r;
    standard_room_price = s_p;
    deluxe_room_price = d_p;
    luxury_room_price = l_p;
    premium_room_price = p_p;
}

Database::Database()
{
    users = l_users();
    hotels = l_hotels();
    comments = l_comments();
}

Database::~Database() {}

void Database::hotel_setup(const HotelInfo& hotel_info)
{
    Hotel* hotel = new Hotel(hotel_info);
    hotels.push_back(hotel);
}
