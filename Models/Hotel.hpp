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
        SP_RatingOverall,
        SP_RatingPersonal
    };
    struct Coordinates
    {
        long double longitude;
        long double latitude;
        Coordinates(long double x, long double y) : longitude(x), latitude(y) {}
    };
    struct RatingInfo
    {
        std::string hotel_id;
        long double location;
        long double cleanliness;
        long double staff;
        long double facilities;
        long double value_for_money;
        long double overall_rating;
        User* user;
        RatingInfo();
        bool operator==(const RatingInfo& second);
    };
    class Room;
    class Rating;
    typedef std::vector<Room*> v_room;
    typedef std::map<User*, Rating*> m_rating;
    typedef std::vector<Rating*> v_rating;

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
    std::pair<long double, long double> get_coordinates();

    bool have_room_with_this_specifications(int type, int quantity, int check_in, int check_out) const;

    long double cost_to_reserve(int type, int quantity) const;
    long double get_avg_price() const;
    long double calculate_avg_weighted(const RatingInfo& weights);

    int get_star() const;
    int comparator(enum SortProperty property, const Hotel* right_side) const;

    void post_rating(RatingInfo& rating_info);
    void set_default_rating_info(RatingInfo& rating_info);

    RatingInfo get_avg_ratings();

    template<typename T>
    static int comparator(const T& left_side, const T& right_side);
private:
    Hotel(const HotelInfo& info);

    v_room prepare_rooms(const HotelInfo& info);

    bool in_range(Room* room, int check_in, int check_out, int type) const;


    template<class room>
    void construct_rooms(v_room& rooms, int quantity, long double price);

    v_room booked_out(User* user, int type, int quantity, int check_in, int check_out);

    long double calculate_avg_price(const HotelInfo& info);

    RatingInfo calculate_avg_ratings();

    void clean_rooms_up();
    void clean_ratings_up();

    std::map<int, long double> get_map_of_rooms_price() const;
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
    long double avg_price;
};

#endif
