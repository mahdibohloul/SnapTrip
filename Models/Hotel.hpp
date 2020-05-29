#ifndef _HOTEL_H
#define _HOTEL_H

#include "Database.hpp"
#include <string>
#include <vector>
#include <map>

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
    info_t to_string();
    float get_avg_price() const;
    info_t get_full_info();
    info_t get_city() const;
    int get_star() const;
    bool have_room_with_this_specifications(int type, int quantity, int check_in, int check_out) const;

private:
    Hotel(const HotelInfo& info);
    v_room prepare_rooms(const HotelInfo& info);
    bool in_range(Room* room, int check_in, int check_out, int type) const;
    template<class room>
    void construct_rooms(v_room& rooms, int quantity, float price);

    info_t get_num_of_rooms();
    info_t get_rooms_price();
private:
    unsigned int hotel_star_rating;
    t_id id;
    info_t property_name;
    info_t hotel_overview;
    info_t property_amenities;
    info_t city;
    Coordinates geo_coordinates;
    info_t image_url;
    v_room rooms;
};

#endif
