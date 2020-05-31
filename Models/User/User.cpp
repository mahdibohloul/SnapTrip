#include "User.hpp"
#include "ReserveCase.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../Filters/CityFilter.hpp"
#include "../Filters/AvgPriceFilter.hpp"
#include "../Filters/StarRangeFilter.hpp"
#include "../Filters/AdvancedFilter.hpp"
#include "../../Exception/Exception.hpp"
using namespace std;

int Database::User::current_id = ConstNames::First_User_Id;

Database::User::FilterInfo::FilterInfo()
{
    m_room_type.insert(make_pair(ConstNames::StandardRoom, Hotel::Room::Room_Class::Standard));
    m_room_type.insert(make_pair(ConstNames::DeluxeRoom, Hotel::Room::Room_Class::Deluxe));
    m_room_type.insert(make_pair(ConstNames::LuxuryRoom, Hotel::Room::Room_Class::Luxury));
    m_room_type.insert(make_pair(ConstNames::PremiumRoom, Hotel::Room::Room_Class::Premium));
    min_star = ConstNames::Default_Star;
    max_star = ConstNames::Default_Star;
    min_price = ConstNames::Default_Price;
    max_price = ConstNames::Default_Price;
    quantity = ConstNames::Default_Quantity;
    check_in = ConstNames::Default_Reservation;
    check_out = ConstNames::Default_Reservation;
    type = Hotel::Room::Room_Class::None;
}

Database::User::ReserveInfo::ReserveInfo()
{
    m_room_type.insert(make_pair(ConstNames::StandardRoom, Hotel::Room::Room_Class::Standard));
    m_room_type.insert(make_pair(ConstNames::DeluxeRoom, Hotel::Room::Room_Class::Deluxe));
    m_room_type.insert(make_pair(ConstNames::LuxuryRoom, Hotel::Room::Room_Class::Luxury));
    m_room_type.insert(make_pair(ConstNames::PremiumRoom, Hotel::Room::Room_Class::Premium));
}

Database::User::User(const UserInfo& info)
{
    this->id = generate_id();
    this->username = info.username;
    this->email = info.email;
    this->password = info.password;
    this->wallet = Wallet();
    this->booked_rooms = l_booked();
    this->filters = m_filter();
    this->id_reserve_list = list<int>();
}

void Database::User::deposit(float amount)
{
    wallet.amount += amount;
    wallet.transactions.push_front(wallet.amount);
}

info_t Database::User::get_account_information(int num_transactions)
{
    ostringstream os;
    os << get_transactions_info(num_transactions);
    return os.str();
}

info_t Database::User::get_transactions_info(int num_transactions)
{
    ostringstream os;
    string delim = ConstNames::Empty_Str;
    if(!wallet.transactions.empty())
    {
        for(auto transaction_itr = wallet.transactions.begin(); transaction_itr != wallet.transactions.end() && num_transactions > ConstNames::Minimum_No_Transactions; transaction_itr++){
            os <<  delim << *transaction_itr;
            num_transactions--;
            delim = ConstNames::New_Line;
        }
    }
    return os.str();
}

info_t Database::User::get_hotels(Database::l_hotels& hotels)
{
    sort_hotels(hotels);
    if(filters.empty())
        return get_hotels_info(hotels);
    else{
        auto filter_itr = filters.begin();
        return get_filtered_hotels(hotels, filter_itr);
    }
}

void Database::User::set_filters(const FilterInfo &filter_info)
{
    if(filter_info.mode == FilterMode::City)
        filters[FilterMode::City] = new CityFilter(filter_info);
    else if(filter_info.mode == FilterMode::AvgPrice)
        filters[FilterMode::AvgPrice] = new AvgPriceFilter(filter_info);
    else if(filter_info.mode == FilterMode::StarRange)
        filters[FilterMode::StarRange] = new StarRangeFilter(filter_info);
    else if(filter_info.mode == FilterMode::Advanced)
        filters[FilterMode::Advanced] = new AdvancedFilter(filter_info);
}

info_t Database::User::get_hotels_info(Database::l_hotels& hotels)
{
    ostringstream os;
    string delim = ConstNames::Empty_Str;
    for(auto hotel_itr = hotels.begin(); hotel_itr != hotels.end(); hotel_itr++)
    {
        os << delim << (*hotel_itr)->to_string();
        delim = ConstNames::New_Line;
    }
    return os.str();
}

info_t Database::User::get_filtered_hotels(Database::l_hotels& hotels, m_filter::iterator& map_itr)
{
    if(map_itr == filters.end()){
        if(hotels.empty())
            throw Exception(ConstNames::Empty_msg);
        return get_hotels_info(hotels);
    }
    ((*map_itr).second)->reset_filter();
    ((*map_itr).second)->filter(hotels);
    Database::l_hotels new_hotels = ((*map_itr).second)->get_hotels();
    map_itr++;
    return get_filtered_hotels(new_hotels, map_itr);
}

void Database::User::delete_filter()
{
    for(auto map_itr = filters.begin(); map_itr != filters.end(); map_itr++)
        delete (*map_itr).second;
    filters.clear();
}

bool Database::User::can_pay(float cost)
{
    if(wallet.amount < cost)
        return false;
    return true;
}

void Database::User::add_reserve_case(Hotel* hotel, Hotel::v_room& b_rooms, int check_in, int check_out, float price)
{
    booked_rooms.push_front(new ReserveCase(hotel, b_rooms, price, check_in, check_out, id_reserve_list));
}

void Database::User::pay_cost(float cost)
{
    wallet.amount -= cost;
    wallet.transactions.push_front(wallet.amount);
}

Database::User::l_booked Database::User::get_reserve() { return booked_rooms; }

void Database::User::delete_reserve(int id)
{
    auto reserve = query_in_reserves(id);
    if(reserve == nullptr)
        throw Exception(ConstNames::Not_Found_msg);
    delete_from_reserves(reserve);
    deposit(reserve->cost * ConstNames::Fifty_Percent);
    delete reserve;
}

Database::User::ReserveCase* Database::User::query_in_reserves(int id)
{
    for(auto reserve_itr = booked_rooms.begin(); reserve_itr != booked_rooms.end(); reserve_itr++)
    {
        if((*reserve_itr)->id == id)
            return *reserve_itr;
    }
    return nullptr;
}

void Database::User::delete_from_reserves(ReserveCase* reserve)
{
    auto want_delete = booked_rooms.begin();
    for(auto reserve_itr = booked_rooms.begin(); reserve_itr != booked_rooms.end(); reserve_itr++)
    {
        if(*reserve_itr == reserve)
        {
            want_delete = reserve_itr;
            break;
        }
    }
    booked_rooms.erase(want_delete);
}

string Database::User::generate_id()
{
    ostringstream os;
    os << ConstNames::User_Id_Prefix << current_id;
    current_id++;
    return os.str();
}

void Database::User::sort_hotels(Database::l_hotels &hotels)
{
    hotels.sort([] (const Hotel* h1, const Hotel* h2)
    {
        return h1->id < h2->id;
    });
}
