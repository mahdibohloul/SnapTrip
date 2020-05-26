#include "Hotel.hpp"
#include "./Rooms/StandardRoom.hpp"
#include "./Rooms/PremiumRoom.hpp"
#include "./Rooms/DeluxeRoom.hpp"
#include "./Rooms/LuxuryRoom.hpp"




Database::Hotel::Hotel(const Database::HotelInfo& info) : geo_coordinates(Coordinates(info.longitude, info.latitude))
{
    this->id = info.unique_id;
    this->hotel_star_rating = info.hotel_rating;
    this->property_name = info.property_name;
    this->property_amenities = info.property_amenities;
    this->city = info.city;
    this->image_url = info.img_url;
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
