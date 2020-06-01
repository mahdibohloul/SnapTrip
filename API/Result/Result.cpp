#include "Result.hpp"
#include "../../Models/User/ReserveCase.hpp"
#include "../../Models/Comment.hpp"
#include <sstream>
#include <iomanip>
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

API::Result::~Result() {}

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
    for(float f : transactions)
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
    for(auto hotel : hotels)
    {
        os << delim << hotel->to_string();
        delim = ConstNames::New_Line;
    }
    return os.str() + ConstNames::New_Line;
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
}
