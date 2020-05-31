#include "ObjectRelational.hpp"
#include "../ConstNames.hpp"
#include "../Exception/Exception.hpp"
#include "../Models/Comment.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

ObjectRelational* ObjectRelational::instance = nullptr;

ObjectRelational* ObjectRelational::get_instance()
{
    if(instance == nullptr){
        instance = new ObjectRelational();
    }
    return instance;
}

void ObjectRelational::release()
{
    delete instance;
    instance = nullptr;
}

ObjectRelational::ObjectRelational()
{
    database = new Database();
    backend = Backend::get_instance();
}

ObjectRelational::~ObjectRelational()
{
    delete database;
    database = nullptr;
}

void ObjectRelational::database_setup(Backend::data_t data)
{
    info_t property_amenities = data[e_property_amenities];
    coordinate_t latitude = stof(data[e_latitude]);
    coordinate_t longitude = stof(data[e_longitude]);
    num_t no_standard_rooms = stoi(data[e_no_s_rooms]);
    num_t no_deluxe_rooms = stoi(data[e_no_d_rooms]);
    num_t no_luxury_rooms = stoi(data[e_no_l_rooms]);
    num_t no_premium_rooms = stoi(data[e_no_p_rooms]);
    price_t standard_room_price = stoi(data[e_s_price]);
    price_t deluxe_room_price = stoi(data[e_d_price]);
    price_t luxury_room_price = stoi(data[e_l_price]);
    price_t premium_room_price = stoi(data[e_p_price]);
    num_t hotel_star_rating = stoi(data[e_star_rating]);

    Database::HotelInfo hotel_info = Database::HotelInfo(data[e_id], data[e_name], hotel_star_rating, data[e_overview], property_amenities, data[e_city], latitude, longitude, data[e_img_url],
                                                        no_standard_rooms, no_deluxe_rooms, no_luxury_rooms, no_premium_rooms,
                                                        standard_room_price, deluxe_room_price, luxury_room_price, premium_room_price);
    database->hotel_setup(hotel_info);
}

Database::User* ObjectRelational::get_specific_user(Database::UserInfo user_info, const string& mode)
{
    if(mode == ConstNames::Signup_Order)
        database->user_setup(user_info);
    return database->query_in_users(user_info);
}

bool ObjectRelational::authentication(Database::UserInfo user_info, const string mode, const Database::l_users& users)
{
    Database::User* user = nullptr;
    if(mode == ConstNames::Signup_Order || mode == ConstNames::User_Authentication)
        user = database->query_in_users(user_info);
    else if(mode == ConstNames::Online_User_Check)
        user = database->query_in_users(user_info, users);
    if(user != nullptr)
        return ConstNames::Exist;
    return !ConstNames::Exist;
}

void ObjectRelational::logout_user(Database::User *user)
{
    user->delete_filter();
}

void ObjectRelational::deposit_in(const Database::UserInfo& user_info, Database::User* user)
{
    database->increase_user_amount(user_info, user);
}

Content ObjectRelational::print_account_information(const Database::UserInfo& user_info, Database::User* user)
{
    return database->print_account_information(user_info, user);
}

Content ObjectRelational::get_hotels(Database::User *user)
{
    return database->get_hotels(user);
}

Content ObjectRelational::get_hotels(info_t hotel_id)
{
    return database->get_hotels(hotel_id);
}

void ObjectRelational::set_filter(const Database::User::FilterInfo &filter_info, Database::User *user)
{
    user->set_filters(filter_info);
}

void ObjectRelational::delete_filter(Database::User *user)
{
    user->delete_filter();
}

Content ObjectRelational::booked_out(Database::User::ReserveInfo& reserve_info, Database::User* user)
{
    Database::Hotel* hotel = database->query_in_hotels(reserve_info.hotel_id);
    if(hotel == nullptr)
        throw Exception(ConstNames::Not_Found_msg);
    if(!hotel->have_room_with_this_specifications(reserve_info.type, reserve_info.quantity, reserve_info.check_in, reserve_info.check_out))
        throw Exception(ConstNames::Not_Enough_Room_msg);
    if(!user->can_pay(hotel->cost_to_reserve(reserve_info.type, reserve_info.quantity)))
        throw Exception(ConstNames::Not_Enough_Credit_msg);
    return database->booked_out(user, hotel, reserve_info.quantity, reserve_info.check_in, reserve_info.check_out, reserve_info.type);
}

Content ObjectRelational::get_reserve(Database::User* user)
{
    return database->get_reserve(user);
}

Content ObjectRelational::delete_reserve(Database::User* user, int id)
{
    user->delete_reserve(id);
    return ConstNames::OK_msg;
}

void ObjectRelational::post_comment(Database::User* user, string hotel_id, string comment)
{
    database->post_comment(user, hotel_id, comment);
}

Content ObjectRelational::get_comment(std::string hotel_id)
{
    auto comments = database->get_comment(hotel_id);
    ostringstream os;
    string delim = ConstNames::Empty_Str;
    if(comments.empty())
        return os.str();
    for(auto comment_itr = comments.begin(); comment_itr != comments.end(); comment_itr++)
    {
        os << delim << (*comment_itr)->get_info();
        delim = ConstNames::New_Line;
    }
    return os.str();
}

void ObjectRelational::post_rating(Database::Hotel::RatingInfo &rating_info)
{
    auto hotel = database->query_in_hotels(rating_info.hotel_id);
    if(hotel == nullptr)
        throw Exception(ConstNames::Not_Found_msg);
    hotel->post_rating(rating_info);
}

Content ObjectRelational::get_rating(std::string hotel_id)
{
    auto hotel = database->query_in_hotels(hotel_id);
    if(hotel == nullptr)
        throw Exception(ConstNames::Not_Found_msg);
    auto avg_ratings = hotel->get_avg_ratings();
    ostringstream os;
    os << setprecision(ConstNames::Precision) << fixed;
    os << ConstNames::Location << ConstNames::Colon << ConstNames::Space << avg_ratings.location << endl;
    os << ConstNames::Cleanliness << ConstNames::Colon << ConstNames::Space << avg_ratings.cleanliness << endl;
    os << ConstNames::Staff << ConstNames::Colon << ConstNames::Space << avg_ratings.staff << endl;
    os << ConstNames::Facilities << ConstNames::Colon << ConstNames::Space << avg_ratings.facilities << endl;
    os << ConstNames::Value_For_Money_sep << ConstNames::Colon << ConstNames::Space << avg_ratings.value_for_money << endl;
    os << ConstNames::Overall_Rating_sep << ConstNames::Colon << ConstNames::Space << avg_ratings.overall_rating;
    return os.str();
}
