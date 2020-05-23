#ifndef _HOTEL_H
#define _HOTEL_H

#include "Database.hpp"
#include <string>
#include <vector>

typedef std::vector<std::string> v_string;

class Database::Hotel
{
public:
    struct Coordinates
    {
        float longitude;
        float latitude;
    };
    class Room;
    typedef std::vector<Room*> v_room;

private:
    Hotel();
    ~Hotel();

private:
    unsigned int hotel_star_rating;
    std::string id;
    std::string property_name;
    std::string hotel_overview;
    v_string property_amenities;
    std::string city;
    Coordinates geo_coordinates;
    std::string image_url;
    v_room rooms;
};

#endif
