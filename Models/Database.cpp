#include "Database.hpp"
#include "Hotel.hpp"
#include "../ConstNames.hpp"
#include "./User/User.hpp"
#include "../Exception/Exception.hpp"
#include <iostream>
using namespace std;

Database::HotelInfo::HotelInfo(info_t u_id, info_t prop_name, num_t hotel_r, info_t hotel_ov, info_t prop_amenities,
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

void Database::user_setup(const UserInfo &user_info)
{
    User* user = new User(user_info);
    users.push_back(user);
}

Database::User* Database::query_in_users(UserInfo user_info)
{
    if(!users.empty())
    {
        for(auto user_itr = users.begin(); user_itr != users.end(); user_itr++)
        {
            if(check_info(*user_itr, user_info) == ConstNames::Equal)
                return *user_itr;
        }
    }
    return nullptr;
}

Database::User* Database::query_in_users(UserInfo user_info, const l_users& users)
{
    if(!users.empty())
    {
        for(auto user_itr = users.begin(); user_itr != users.end(); user_itr++)
        {
            if(check_info(*user_itr, user_info) == ConstNames::Equal)
                return *user_itr;
        }
    }
    return nullptr;
}

Database::Hotel* Database::query_in_hotels(info_t hotel_id)
{
    if(hotel_id != ConstNames::Empty_Str)
    {
        for(auto hotel_itr = hotels.begin(); hotel_itr != hotels.end(); hotel_itr++)
        {
            if((*hotel_itr)->id == hotel_id)
                return *hotel_itr;
        }
    }
    return nullptr;
}

bool Database::check_info(User* current_user, UserInfo user_info)
{
    if(current_user->email == user_info.email || current_user->username == user_info.username)
        return ConstNames::Equal;
    return !ConstNames::Equal;
}

void Database::increase_user_amount(const UserInfo &user_info, User *user)
{
    user->deposit(user_info.amount);
}

info_t Database::print_account_information(const UserInfo &user_info, User *user)
{
    return user->get_account_information(user_info.no_transactions);
}

info_t Database::get_hotels(User* user)
{
    if(hotels.empty())
        throw Exception(ConstNames::Empty_msg);
    return user->get_hotels(hotels);
}

info_t Database::get_hotels(info_t hotel_id)
{
    Hotel* hotel = query_in_hotels(hotel_id);
    if(hotel == nullptr)
        throw Exception(ConstNames::Not_Found_msg);
    return hotel->get_full_info();
}
