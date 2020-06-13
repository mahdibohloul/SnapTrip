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

Database::Hotel::~Hotel()
{
    clean_rooms_up();
    clean_ratings_up();
}

info_t Database::Hotel::to_string() const
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

Database::Hotel::v_room Database::Hotel::booked_out(User* user, int type, int quantity, int check_in, int check_out)
{
    v_room booked_rooms = v_room();
    int q = 0;
    for(auto room_itr = rooms.begin(); room_itr != rooms.end() && q < quantity; room_itr++)
    {
        if(in_range(*room_itr, check_in, check_out, type)){
            q++;
            (*room_itr)->booked(check_in, check_out);
            booked_rooms.push_back(*room_itr);
        }
    }
    int night_stay = ((check_out - check_in) < ConstNames::One_Night) ? ConstNames::One_Night : (check_out - check_in);
    user->pay_cost(night_stay * cost_to_reserve(type, quantity));
    user->add_reserve_case(this, booked_rooms, check_in, check_out, night_stay * cost_to_reserve(type, quantity));
    return booked_rooms;
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
    if(q == 0)
        return 0.0f;
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

map<int, float> Database::Hotel::get_map_of_rooms_price() const
{
    map<int, float> map_of_price;
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
    return map_of_price;
}

info_t Database::Hotel::get_rooms_price()
{
    auto map_of_price = get_map_of_rooms_price();
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

void Database::Hotel::clean_rooms_up()
{
    for(auto room_itr = rooms.begin(); room_itr != rooms.end(); room_itr++)
    {
        delete *room_itr;
        *room_itr = nullptr;
    }
    rooms.clear();
}

void Database::Hotel::clean_ratings_up()
{
    for(auto rating_itr = map_ratings.begin(); rating_itr != map_ratings.end(); rating_itr++)
    {
        delete (*rating_itr).second;
        (*rating_itr).second = nullptr;
    }
    map_ratings.clear();
}

info_t Database::Hotel::get_id() { return id; }

info_t Database::Hotel::get_name() {return property_name; }

int Database::Hotel::get_hotel_star() { return hotel_star_rating; }

info_t Database::Hotel::get_hotel_overview() {return hotel_overview; }

info_t Database::Hotel::get_amenities() { return property_amenities; }

pair<float, float> Database::Hotel::get_coordinates() { return make_pair(geo_coordinates.latitude, geo_coordinates.longitude); }

int Database::Hotel::comparator(enum SortProperty property, const Hotel *right_side) const
{
    auto map = get_map_of_rooms_price();
    auto rs_map = right_side->get_map_of_rooms_price();
    switch(property)
    {
        case SP_Id : return comparator(id, right_side->id);
        case SP_Name : return comparator(property_name, right_side->property_name);
        case SP_Star_Rating : return comparator(hotel_star_rating, right_side->hotel_star_rating);
        case SP_City : return comparator(city, right_side->city);
        case SP_StandardRoomPrice : return comparator(map[Room::Room_Class::Standard], rs_map[Room::Room_Class::Standard]);
        case SP_DeluxeRoomPrice : return comparator(map[Room::Room_Class::Deluxe], rs_map[Room::Room_Class::Deluxe]);
        case SP_LuxuryRoomPrice : return comparator(map[Room::Room_Class::Luxury], rs_map[Room::Room_Class::Luxury]);
        case SP_PremiumRoomPrice : return comparator(map[Room::Room_Class::Premium], rs_map[Room::Room_Class::Premium]);
        case SP_AvgRoomPrice : return comparator(avg_price, right_side->avg_price);
    }
}

template<typename T>
int Database::Hotel::comparator(const T& left_side, const T& hand_side)
{
    return left_side < hand_side ? ConstNames::Smaller_cmp : left_side == hand_side ? ConstNames::Equal_cmp : ConstNames::Greater_cmp;
}
