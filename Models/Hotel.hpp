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
    enum SortProperty
    {
        SP_Id,
        SP_Name,
        SP_Star_Rating,
        SP_City,
        SP_StandardRoomPrice,
        SP_DeluxeRoomPrice,
        SP_LuxuryRoomPrice,
        SP_PremiumRoomPrice,
        SP_AvgRoomPrice,
        SP_RatingOverall
    };
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
        bool operator==(const RatingInfo& second);
    };
    class Room;
    class Rating;
    typedef std::vector<Room*> v_room;
    typedef std::map<User*, Rating*> m_rating;

public:
    ~Hotel();

    info_t to_string() const;
    info_t get_city() const;
    info_t get_id();
    info_t get_name();
    int get_hotel_star();
    info_t get_hotel_overview();
    info_t get_amenities();
    info_t get_num_of_rooms();
    info_t get_rooms_price();
    std::pair<float, float> get_coordinates();

    bool have_room_with_this_specifications(int type, int quantity, int check_in, int check_out) const;

    float cost_to_reserve(int type, int quantity) const;
    float get_avg_price() const;

    int get_star() const;
    int comparator(enum SortProperty property, const Hotel* right_side) const;

    void post_rating(RatingInfo& rating_info);
    void set_default_rating_info(RatingInfo& rating_info);

    RatingInfo get_avg_ratings();

private:
    Hotel(const HotelInfo& info);

    v_room prepare_rooms(const HotelInfo& info);

    bool in_range(Room* room, int check_in, int check_out, int type) const;

    template<typename T>
    static int comparator(const T& left_side, const T& right_side);

    template<class room>
    void construct_rooms(v_room& rooms, int quantity, float price);

    v_room booked_out(User* user, int type, int quantity, int check_in, int check_out);

    float calculate_avg_price(const HotelInfo& info);

    RatingInfo calculate_avg_ratings();

    void clean_rooms_up();
    void clean_ratings_up();

    std::map<int, float> get_map_of_rooms_price() const;
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
    RatingInfo default_rating;
    int hotel_star_rating;
    float avg_price;
};

#endif
