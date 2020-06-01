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
    friend class Database;
    struct Coordinates
    {
        float longitude;
        float latitude;
        Coordinates(float x, float y) : longitude(x), latitude(y) {}
    };
    struct RatingInfo
    {
        std::string hotel_id;
        float location;
        float cleanliness;
        float staff;
        float facilities;
        float value_for_money;
        float overall_rating;
        User* user;
        RatingInfo();
    };
    class Room;
    class Rating;
    typedef std::vector<Room*> v_room;
    typedef std::map<User*, Rating*> m_rating;

public:
    ~Hotel();

    info_t to_string();
    info_t get_full_info();
    info_t get_city() const;

    bool have_room_with_this_specifications(int type, int quantity, int check_in, int check_out) const;

    float cost_to_reserve(int type, int quantity) const;
    float get_avg_price() const;

    int get_star() const;

    void post_rating(RatingInfo& rating_info);

    RatingInfo get_avg_ratings();

private:
    Hotel(const HotelInfo& info);

    v_room prepare_rooms(const HotelInfo& info);

    bool in_range(Room* room, int check_in, int check_out, int type) const;

    template<class room>
    void construct_rooms(v_room& rooms, int quantity, float price);

    info_t get_num_of_rooms();
    info_t get_rooms_price();
    info_t booked_out(User* user, int type, int quantity, int check_in, int check_out);

    float calculate_avg_price(const HotelInfo& info);

    RatingInfo calculate_avg_ratings();

    void clean_rooms_up();
    void clean_ratings_up();
private:
    t_id id;
    info_t property_name;
    info_t hotel_overview;
    info_t property_amenities;
    info_t city;
    Coordinates geo_coordinates;
    info_t image_url;
    v_room rooms;
    m_rating map_ratings;
    int hotel_star_rating;
    float avg_price;
};

#endif
