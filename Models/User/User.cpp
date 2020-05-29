#include "User.hpp"
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

Database::User::User(const UserInfo& info)
{
    this->id = generate_id();
    this->username = info.username;
    this->email = info.email;
    this->password = info.password;
    wallet = Wallet();
    booked_rooms = v_booked();
    filters = m_filter();
}

string Database::User::generate_id()
{
    ostringstream os;
    os << ConstNames::User_Id_Prefix << current_id;
    current_id++;
    return os.str();
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
    ostringstream os;
    sort_hotels(hotels);
    if(filters.empty())
        os << get_hotels_info(hotels);
    else{
        auto filter_itr = filters.begin();
        os << get_filtered_hotels(hotels, filter_itr);
    }
    return os.str();
}

void Database::User::sort_hotels(Database::l_hotels &hotels)
{
    hotels.sort([] (const Hotel* h1, const Hotel* h2)
    {
        return h1->id < h2->id;
    });
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
