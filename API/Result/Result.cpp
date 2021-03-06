#include "Result.hpp"
#include "../../Models/User/ReserveCase.hpp"
#include "../../Models/Comment.hpp"
#include <sstream>
#include <iomanip>
#include <iostream>


#define Location_ES 0
#define Cleanliness_ES 1
#define Staff_ES 2
#define Facilities_ES 3
#define Value_ES 4
using namespace std;

API::Result::Result(string msg)
{
    this->result_msg = msg;
    this->type = Message;
    construct_map();
}

API::Result::Result(Database::Hotel::v_room booked_rooms)
{
    this->booked_rooms = booked_rooms;
    this->type = Booking;
    construct_map();
}

API::Result::Result(Database::User::l_transactions transactions)
{
    this->transactions = transactions;
    this->type = Transactions;
    construct_map();
}

API::Result::Result(Database::User::l_booked reservations)
{
    this->reservations = reservations;
    this->type = Reservations;
    construct_map();
}

API::Result::Result(Database::Hotel* hotel)
{
    this->hotel = hotel;
    this->type = Hotel_Full_Info;
    construct_map();
}

API::Result::Result(Database::l_hotels hotels)
{
    this->hotels = hotels;
    this->type = Hotels;
    construct_map();
}

API::Result::Result(Database::l_comments comments)
{
    this->comments = comments;
    this->type = GET_Comments;
    construct_map();
}

API::Result::Result(Database::Hotel::RatingInfo avg_ratings)
{
    this->average_ratings = avg_ratings;
    this->type = Average_Ratings;
    construct_map();
}

API::Result::Result(Database::User::Weights manual_weights)
{
    this->manual_weights = manual_weights;
    this->type = Manual_Weights;
    construct_map();
}

API::Result::Result(v_double estimated_weights)
{
    this->estimated_weights = estimated_weights;
    this->type = Estimated_Weights;
}

API::Result::~Result()
{
    comments.clear();
    hotels.clear();
    reservations.clear();
    transactions.clear();
    booked_rooms.clear();
}

string API::Result::get_result()
{
    return (this->*map_of_results[type])();
}

string API::Result::get_message() { return result_msg + ConstNames::New_Line; }

string API::Result::get_booked_rooms_id()
{
    ostringstream os;
    string delim;
    for(auto room : booked_rooms)
    {
        os << delim << room->get_id();
        delim = ConstNames::Space;
    }
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_transactions_info()
{
    ostringstream os;
    string delim;
    for(long double f : transactions)
    {
        os << delim << f;
        delim = ConstNames::New_Line;
    }
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_reservations_info()
{
    ostringstream os;
    string delim = ConstNames::Empty_Str;
    for(auto reserve_itr = reservations.begin(); reserve_itr != reservations.end(); reserve_itr++)
    {
        pair<int, int> reserved_days = (*reserve_itr)->get_reserved_days();
        os << delim << ConstNames::ID << ConstNames::Colon << ConstNames::Space << (*reserve_itr)->get_reserve_id() << ConstNames::Space;
        os << ConstNames::Hotel << ConstNames::Colon << ConstNames::Space << (*reserve_itr)->get_hotel_reserved_id() << ConstNames::Space;
        os << ConstNames::Room << ConstNames::Colon << ConstNames::Space << (*reserve_itr)->get_type_of_reserved_rooms() << ConstNames::Space;
        os << ConstNames::Quantity << ConstNames::Colon << ConstNames::Space << (*reserve_itr)->get_quantity_of_reserves() << ConstNames::Space;
        os << ConstNames::Cost << ConstNames::Colon << ConstNames::Space << (*reserve_itr)->get_cost_of_reserves() << ConstNames::Space;
        os << ConstNames::Check_In << ConstNames::Space << reserved_days.first << ConstNames::Space;
        os << ConstNames::Check_Out << ConstNames::Space << reserved_days.second;
        delim = ConstNames::New_Line;
    }
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_hotel_full_info()
{
    ostringstream os;
    auto coordinates = hotel->get_coordinates();
    os << hotel->get_id() << endl;
    os << hotel->get_name() << endl;
    os << setprecision(ConstNames::Precision) << fixed;
    os << ConstNames::Star << ConstNames::Colon << ConstNames::Space << hotel->get_star() << endl;
    os << ConstNames::Overview << ConstNames::Colon << ConstNames::Space << hotel->get_hotel_overview() << endl;
    os << ConstNames::Amenities << ConstNames::Colon << ConstNames::Space << hotel->get_amenities() << endl;
    os << ConstNames::City << ConstNames::Colon << ConstNames::Space << hotel->get_city() << endl;
    os << ConstNames::Latitude << ConstNames::Colon << ConstNames::Space << coordinates.first << endl;
    os << ConstNames::Longitude << ConstNames::Colon << ConstNames::Space << coordinates.second << endl;
    os << ConstNames::Hashtag << ConstNames::Rooms << ConstNames::Colon << ConstNames::Space << hotel->get_num_of_rooms() << endl;
    os << ConstNames::Price << ConstNames::Colon << ConstNames::Space << hotel->get_rooms_price();
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_hotels_info()
{
    ostringstream os;
    string delim;
    os << get_extra_info();
    for(auto hotel : hotels)
    {
        os << delim << hotel->to_string() << ConstNames::Space;
        delim = ConstNames::New_Line;
    }
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_extra_info()
{
    ostringstream os;
    auto backend = Backend::get_instance();
    auto user = backend->get_curr_user();
    if(user->the_default_filter_applied())
        os << ConstNames::Default_Filter_msg << endl;
    return os.str();
}

string API::Result::get_comments_info()
{
    if(comments.empty())
        return ConstNames::Empty_Str;
    ostringstream os;
    string delim = ConstNames::Empty_Str;
    if(comments.empty())
        return os.str();
    for(auto comment_itr = comments.begin(); comment_itr != comments.end(); comment_itr++)
    {
        os << delim << (*comment_itr)->get_writer() << ConstNames::Colon << ConstNames::Space << (*comment_itr)->get_comment();
        delim = ConstNames::New_Line;
    }
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_average_ratings()
{
    ostringstream os;
    os << setprecision(ConstNames::Precision) << fixed;
    os << ConstNames::Location << ConstNames::Colon << ConstNames::Space << average_ratings.location << endl;
    os << ConstNames::Cleanliness << ConstNames::Colon << ConstNames::Space << average_ratings.cleanliness << endl;
    os << ConstNames::Staff << ConstNames::Colon << ConstNames::Space << average_ratings.staff << endl;
    os << ConstNames::Facilities << ConstNames::Colon << ConstNames::Space << average_ratings.facilities << endl;
    os << ConstNames::Value_For_Money_sep << ConstNames::Colon << ConstNames::Space << average_ratings.value_for_money << endl;
    os << ConstNames::Overall_Rating_sep << ConstNames::Colon << ConstNames::Space << average_ratings.overall_rating;
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_manual_weights()
{
    ostringstream os;
    os << setprecision(ConstNames::Precision) << fixed;
    os << ConstNames::Active << ConstNames::Space << (manual_weights.activity == true ? ConstNames::True : ConstNames::False);
    if(manual_weights.activity)
    {
        os << ConstNames::Space << ConstNames::Location << ConstNames::Space << manual_weights.weights.location << ConstNames::Space;
        os << ConstNames::Cleanliness << ConstNames::Space << manual_weights.weights.cleanliness << ConstNames::Space;
        os << ConstNames::Staff << ConstNames::Space << manual_weights.weights.staff << ConstNames::Space;
        os << ConstNames::Facilities << ConstNames::Space << manual_weights.weights.facilities << ConstNames::Space;
        os << ConstNames::Value_For_Money_sep << ConstNames::Space << manual_weights.weights.value_for_money;
    }
    return os.str() + ConstNames::New_Line;
}

string API::Result::get_estimated_weights()
{
    ostringstream os;
    os << setprecision(ConstNames::Precision) << fixed;
    os << ConstNames::Location << ConstNames::Space << estimated_weights[Location_ES] << ConstNames::Space;
    os << ConstNames::Cleanliness << ConstNames::Space << estimated_weights[Cleanliness_ES] << ConstNames::Space;
    os << ConstNames::Staff << ConstNames::Space << estimated_weights[Staff_ES] << ConstNames::Space;
    os << ConstNames::Facilities << ConstNames::Space << estimated_weights[Facilities_ES] << ConstNames::Space;
    os << ConstNames::Value_For_Money << ConstNames::Space << estimated_weights[Value_ES];
    return os.str() + ConstNames::New_Line;
}


void API::Result::construct_map()
{
    map_of_results.insert(make_pair(Message, &API::Result::get_message));
    map_of_results.insert(make_pair(Booking, &API::Result::get_booked_rooms_id));
    map_of_results.insert(make_pair(Transactions, &API::Result::get_transactions_info));
    map_of_results.insert(make_pair(Reservations, &API::Result::get_reservations_info));
    map_of_results.insert(make_pair(Hotel_Full_Info, &API::Result::get_hotel_full_info));
    map_of_results.insert(make_pair(Hotels, &API::Result::get_hotels_info));
    map_of_results.insert(make_pair(GET_Comments, &API::Result::get_comments_info));
    map_of_results.insert(make_pair(Average_Ratings, &API::Result::get_average_ratings));
    map_of_results.insert(make_pair(Manual_Weights, &API::Result::get_manual_weights));
    map_of_results.insert(make_pair(Estimated_Weights, &API::Result::get_estimated_weights));
}
