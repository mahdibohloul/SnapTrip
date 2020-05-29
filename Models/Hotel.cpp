#include "Hotel.hpp"
#include "./Rooms/StandardRoom.hpp"
#include "./Rooms/PremiumRoom.hpp"
#include "./Rooms/DeluxeRoom.hpp"
#include "./Rooms/LuxuryRoom.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;




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

info_t Database::Hotel::to_string()
{
    ostringstream os;
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
    os << Star << ConstNames::Colon << ConstNames::Space << hotel_star_rating << endl;
    os << Overview << ConstNames::Colon << ConstNames::Space << hotel_overview << endl;
    os << Amenities << ConstNames::Colon << ConstNames::Space << property_amenities << endl;
    os << City << ConstNames::Colon << ConstNames::Space << city << endl;
    os << Latitude << ConstNames::Colon << ConstNames::Space << geo_coordinates.latitude << endl;
    os << Longitude << ConstNames::Colon << ConstNames::Space << geo_coordinates.longitude << endl;
    os << ConstNames::Hashtag << ConstNames::Room << ConstNames::Colon << ConstNames::Space << get_num_of_rooms() << endl;
    os << Price << ConstNames::Colon << ConstNames::Space << get_rooms_price();
    return os.str();
}

float Database::Hotel::get_avg_price()
{
    map<Room::Room_Class, float> map_of_price;
    for(auto room_itr = rooms.begin(); room_itr != rooms.end(); room_itr++)
    {
        Room::Room_Class type = (*room_itr)->type;
        float price = (*room_itr)->price;
        map_of_price.insert(make_pair(type, price));
    }
    float sum = 0;
    for(auto map_itr = map_of_price.begin(); map_itr != map_of_price.end(); map_itr++)
        sum += (*map_itr).second;
    return sum / map_of_price.size();
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
