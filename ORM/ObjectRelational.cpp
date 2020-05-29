#include "ObjectRelational.hpp"
#include "../ConstNames.hpp"
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
