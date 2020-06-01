#ifndef _RESULT_H
#define _RESULT_H

#include "../API.hpp"

class API::Result
{
public:
    friend class API;
    enum ResultType
    {
        Message,
        Booking,
        Transactions,
        Reservations,
        Hotel_Full_Info,
        Hotels,
        GET_Comments,
        Average_Ratings
    };
    typedef std::string (API::Result::*result_method)(void);

public:
    ~Result();

private:
    Result(std::string msg);
    Result(Database::Hotel::v_room booked_rooms);
    Result(Database::User::l_transactions transactions);
    Result(Database::User::l_booked reservations);
    Result(Database::Hotel* hotel);
    Result(Database::l_hotels hotels);
    Result(Database::l_comments comments);
    Result(Database::Hotel::RatingInfo avg_ratings);

    void construct_map();

    std::string get_result();
    std::string get_message();
    std::string get_booked_rooms_id();
    std::string get_transactions_info();
    std::string get_reservations_info();
    std::string get_hotel_full_info();
    std::string get_hotels_info();
    std::string get_comments_info();
    std::string get_average_ratings();

private:
    std::string result_msg;
    Database::Hotel::v_room booked_rooms;
    Database::User::l_transactions transactions;
    Database::User::l_booked reservations;
    Database::Hotel* hotel;
    Database::l_hotels hotels;
    Database::l_comments comments;
    Database::Hotel::RatingInfo average_ratings;
    ResultType type;
    std::map<ResultType, result_method> map_of_results;

};

#endif
