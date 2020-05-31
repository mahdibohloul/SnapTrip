#include "Hotel.hpp"
#include "./Rooms/StandardRoom.hpp"
#include "./Rooms/PremiumRoom.hpp"
#include "./Rooms/DeluxeRoom.hpp"
#include "./Rooms/LuxuryRoom.hpp"
#include "User/User.hpp"
#include "../Exception/Exception.hpp"
#include "Rating.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <map>
using namespace std;



Database::Hotel::RatingInfo::RatingInfo()
{
    hotel_id = ConstNames::Empty_Str;
    location = 0.0f;
    cleanliness = 0.0f;
    staff = 0.0f;
    facilities = 0.0f;
    value_for_money = 0.0f;
    overall_rating = 0.0f;
    user = nullptr;
}

Database::Hotel::Hotel(const Database::HotelInfo& info) : geo_coordinates(Coordinates(info.longitude, info.latitude))
{
    this->id = info.unique_id;
    this->hotel_star_rating = info.hotel_rating;
    this->property_name = info.property_name;
    this->property_amenities = info.property_amenities;
    this->city = info.city;
    this->image_url = info.img_url;
    this->hotel_overview = info.hotel_overview;
    this->rooms = prepare_rooms(info);
    this->map_ratings = m_rating();
    avg_price = calculate_avg_price(info);
}

info_t Database::Hotel::to_string()
{
    ostringstream os;
    os << setprecision(ConstNames::Precision) << fixed;
    os << id << ConstNames::Space_str;
    os << property_name << ConstNames::Space_str;
    os << hotel_star_rating << ConstNames::Space_str;
    os << city << ConstNames::Space_str;
    os << rooms.size() << ConstNames::Space_str;
    os << get_avg_price();
    return os.str();
}

info_t Database::Hotel::get_full_info()
{
    ostringstream os;
    os << id << endl;
    os << property_name << endl;
    os << setprecision(ConstNames::Precision) << fixed;
    os << ConstNames::Star << ConstNames::Colon << ConstNames::Space << hotel_star_rating << endl;
    os << ConstNames::Overview << ConstNames::Colon << ConstNames::Space << hotel_overview << endl;
    os << ConstNames::Amenities << ConstNames::Colon << ConstNames::Space << property_amenities << endl;
    os << ConstNames::City << ConstNames::Colon << ConstNames::Space << city << endl;
    os << ConstNames::Latitude << ConstNames::Colon << ConstNames::Space << geo_coordinates.latitude << endl;
    os << ConstNames::Longitude << ConstNames::Colon << ConstNames::Space << geo_coordinates.longitude << endl;
    os << ConstNames::Hashtag << ConstNames::Rooms << ConstNames::Colon << ConstNames::Space << get_num_of_rooms() << endl;
    os << ConstNames::Price << ConstNames::Colon << ConstNames::Space << get_rooms_price();
    return os.str();
}

void Database::Hotel::post_rating(RatingInfo &rating_info)
{
    auto rating = map_ratings.find(rating_info.user);
    if(rating != map_ratings.end())
    {
        delete rating->second;
        rating->second = new Rating(rating_info);
    }
    else
        map_ratings.insert(make_pair(rating_info.user, new Rating(rating_info)));
}

bool Database::Hotel::have_room_with_this_specifications(int type, int quantity, int check_in, int check_out) const
{
    int q = 0;
    for(auto room_itr = rooms.begin(); room_itr != rooms.end(); room_itr++)
    {
        if(in_range(*room_itr, check_in, check_out, type))
            q++;
    }
    if(q >= quantity)
        return true;
    return false;
}

float Database::Hotel::cost_to_reserve(int type, int quantity) const
{
    float cost = 0.0f;
    int q = 0;
    for(auto room_itr = rooms.begin(); room_itr != rooms.end() && q < quantity; room_itr++)
    {
        if((*room_itr)->type == type){
            q++;
            cost += (*room_itr)->price;
        }
    }
    return cost;
}

bool Database::Hotel::in_range(Room* room, int check_in, int check_out, int type) const
{
    if(room->type == type)
    {
        if(room->can_reserve(check_in, check_out))
            return true;
    }
    return false;
}

info_t Database::Hotel::booked_out(User* user, int type, int quantity, int check_in, int check_out)
{
    v_room booked_rooms = v_room();
    ostringstream os;
    string delim;
    int q = 0;
    for(auto room_itr = rooms.begin(); room_itr != rooms.end() && q < quantity; room_itr++)
    {
        if(in_range(*room_itr, check_in, check_out, type)){
            q++;
            (*room_itr)->booked(check_in, check_out);
            os << delim << (*room_itr)->id;
            delim = ConstNames::Space;
            booked_rooms.push_back(*room_itr);
        }
    }
    int night_stay = ((check_out - check_in) < ConstNames::One_Night) ? ConstNames::One_Night : (check_out - check_in);
    user->pay_cost(night_stay * cost_to_reserve(type, quantity));
    user->add_reserve_case(this, booked_rooms, check_in, check_out, night_stay * cost_to_reserve(type, quantity));
    return os.str();
}

Database::Hotel::RatingInfo Database::Hotel::get_avg_ratings()
{
    if(map_ratings.empty())
        throw Exception(ConstNames::No_Rating_msg);
    return calculate_avg_ratings();
}

Database::Hotel::RatingInfo Database::Hotel::calculate_avg_ratings()
{
    RatingInfo rating_info = RatingInfo();
    for(auto rating_itr = map_ratings.begin(); rating_itr != map_ratings.end(); rating_itr++)
    {
        rating_info.location += ((*rating_itr).second->location / map_ratings.size());
        rating_info.cleanliness += ((*rating_itr).second->cleanliness / map_ratings.size());
        rating_info.staff += ((*rating_itr).second->staff / map_ratings.size());
        rating_info.facilities += ((*rating_itr).second->facilities / map_ratings.size());
        rating_info.value_for_money += ((*rating_itr).second->value_for_money / map_ratings.size());
        rating_info.overall_rating += ((*rating_itr).second->overall_rating / map_ratings.size());
    }
    return rating_info;
}

float Database::Hotel::calculate_avg_price(const HotelInfo& info)
{
    vector<float> prices {info.standard_room_price, info.deluxe_room_price, info.luxury_room_price, info.premium_room_price};
    float sum = 0;
    int q = 0;
    for(float f : prices)
    {
        if(f != 0)
        {
            sum += f;
            q++;
        }
    }
    return sum/q;
}

Database::Hotel::v_room Database::Hotel::prepare_rooms(const Database::HotelInfo& info)
{
    v_room rooms = v_room();
    construct_rooms<StandardRoom>(rooms, info.no_standard_rooms, info.standard_room_price);
    construct_rooms<DeluxeRoom>(rooms, info.no_deluxe_rooms, info.deluxe_room_price);
    construct_rooms<LuxuryRoom>(rooms, info.no_luxury_rooms, info.luxury_room_price);
    construct_rooms<PremiumRoom>(rooms, info.no_premium_rooms, info.premium_room_price);
    Room::reset_room_id(rooms);
    return rooms;
}

template<class room>
void Database::Hotel::construct_rooms(v_room& rooms, int quantity, float price)
{
    if(quantity == ConstNames::No_Room)
        return;
    for(num_t count = 0; count < quantity; count++)
        rooms.push_back(new room(price));

}

float Database::Hotel::get_avg_price() const
{
    return avg_price;
}

info_t Database::Hotel::get_rooms_price()
{
    map<Room::Room_Class, float> map_of_price;
    map_of_price.insert(make_pair(Room::Room_Class::Standard, 0));
    map_of_price.insert(make_pair(Room::Room_Class::Deluxe, 0));
    map_of_price.insert(make_pair(Room::Room_Class::Luxury, 0));
    map_of_price.insert(make_pair(Room::Room_Class::Premium, 0));
    for(auto room_itr = rooms.begin(); room_itr != rooms.end(); room_itr++)
    {
        Room::Room_Class type = (*room_itr)->type;
        float price = (*room_itr)->price;
        map_of_price[type] = price;
    }
    ostringstream os;
    os << map_of_price[Room::Room_Class::Standard] << ConstNames::Space << map_of_price[Room::Room_Class::Deluxe] << ConstNames::Space << map_of_price[Room::Room_Class::Luxury] << ConstNames::Space << map_of_price[Room::Room_Class::Premium];
    return os.str();
}

info_t Database::Hotel::get_num_of_rooms()
{
    map<Room::Room_Class, int> map_of_rooms;
    map_of_rooms.insert(make_pair(Room::Room_Class::Standard, 0));
    map_of_rooms.insert(make_pair(Room::Room_Class::Deluxe, 0));
    map_of_rooms.insert(make_pair(Room::Room_Class::Luxury, 0));
    map_of_rooms.insert(make_pair(Room::Room_Class::Premium, 0));

    for(auto room_itr = rooms.begin(); room_itr != rooms.end(); room_itr++)
    {
        Room::Room_Class type = (*room_itr)->type;
        map_of_rooms[type]++;
    }

    ostringstream os;
    os << map_of_rooms[Room::Room_Class::Standard] << ConstNames::Space << map_of_rooms[Room::Room_Class::Deluxe] << ConstNames::Space << map_of_rooms[Room::Room_Class::Luxury] << ConstNames::Space << map_of_rooms[Room::Room_Class::Premium];
    return os.str();
}

info_t Database::Hotel::get_city() const { return city; }

int Database::Hotel::get_star() const { return hotel_star_rating; }
