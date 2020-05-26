#ifndef _HOTEL_H
#define _HOTEL_H

#include "Database.hpp"
#include <string>
#include <vector>

typedef std::vector<std::string> v_string;
typedef std::string t_id;

class Database::Hotel
{
public:
    struct Coordinates
    {
        float longitude;
        float latitude;
        Coordinates(float x, float y) : longitude(x), latitude(y) {}
    };
    class Room;
    class Rating;
    typedef std::vector<Room*> v_room;
    typedef std::vector<Rating*> v_rating;
    friend class Database;

public:
    ~Hotel();

private:
    Hotel(const HotelInfo& info);
    v_room prepare_rooms(const HotelInfo& info);

    template<class room>
    void construct_rooms(v_room& rooms, int quantity, float price);

private:
    unsigned int hotel_star_rating;
    t_id id;
    std::string property_name;
    std::string hotel_overview;
    v_string property_amenities;
    std::string city;
    Coordinates geo_coordinates;
    std::string image_url;
    v_room rooms;
};

#endif
