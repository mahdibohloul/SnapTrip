#include "User.hpp"
#include "ReserveCase.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include "../Filters/CityFilter.hpp"
#include "../Filters/AvgPriceFilter.hpp"
#include "../Filters/StarRangeFilter.hpp"
#include "../Filters/AdvancedFilter.hpp"
#include "../Filters/DefaultAvgPriceFilter.hpp"
#include "../../Exception/Exception.hpp"
#include "../../Business-Logic/AI/AI.hpp"
#include "../Rating.hpp"
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

Database::User::SortInfo::SortInfo()
{
    map_sort_property = {
        make_pair(ConstNames::ID, Hotel::SortProperty::SP_Id),
        make_pair(ConstNames::Star_Rating, Hotel::SortProperty::SP_Star_Rating),
        make_pair(ConstNames::Name, Hotel::SortProperty::SP_Name),
        make_pair(ConstNames::City, Hotel::SortProperty::SP_City),
        make_pair(ConstNames::Standard_Room_Price, Hotel::SortProperty::SP_StandardRoomPrice),
        make_pair(ConstNames::Deluxe_Room_Price, Hotel::SortProperty::SP_DeluxeRoomPrice),
        make_pair(ConstNames::Luxury_Room_Price, Hotel::SortProperty::SP_LuxuryRoomPrice),
        make_pair(ConstNames::Premium_Room_Price, Hotel::SortProperty::SP_PremiumRoomPrice),
        make_pair(ConstNames::Average_Room_Price, Hotel::SortProperty::SP_AvgRoomPrice),
        make_pair(ConstNames::Rating_Overall, Hotel::SortProperty::SP_RatingOverall),
        make_pair(ConstNames::Rating_Personal, Hotel::SortProperty::SP_RatingPersonal)
    };
    property = Hotel::SortProperty::SP_Id;
    mode = SortMode::SM_Ascending;
}

Database::User::ReserveInfo::ReserveInfo()
{
    m_room_type.insert(make_pair(ConstNames::StandardRoom, Hotel::Room::Room_Class::Standard));
    m_room_type.insert(make_pair(ConstNames::DeluxeRoom, Hotel::Room::Room_Class::Deluxe));
    m_room_type.insert(make_pair(ConstNames::LuxuryRoom, Hotel::Room::Room_Class::Luxury));
    m_room_type.insert(make_pair(ConstNames::PremiumRoom, Hotel::Room::Room_Class::Premium));
}

bool Database::User::Weights::operator==(const Weights &second_weights)
{
    if(activity == second_weights.activity && weights == second_weights.weights)
        return true;
    return false;
}

long double Database::User::Weights::operator[](const int which)
{
    switch(which)
    {
        case WE_Location: return weights.location;
        case WE_Cleanliness : return weights.cleanliness;
        case WE_Staff : return weights.staff;
        case WE_Facilities : return weights.facilities;
        case WE_Value : return weights.value_for_money;
        default: return 0;
    }
}

// Database::User::Weights& Database::User::Weights::operator=(const Weights &second)
// {

// }

Database::User::Weights& Database::User::Weights::increase_this_var(const long double amount, const int which)
{
    switch(which)
    {
        case WE_Location:
            (*this).weights.location += amount;
            break;
        case WE_Cleanliness:
            (*this).weights.cleanliness += amount;
            break;
        case WE_Staff:
            (*this).weights.staff += amount;
            break;
        case WE_Facilities:
            (*this).weights.staff += amount;
            break;
        case WE_Value:
            (*this).weights.value_for_money += amount;
            break;
    }
    return *this;
}

Database::User::Weights& Database::User::Weights::operator-(const v_double numbers)
{
    weights.location -= numbers[0];
    weights.cleanliness -= numbers[1];
    weights.staff -= numbers[2];
    weights.facilities -= numbers[3];
    weights.value_for_money -= numbers[4];
    return *this;
}


void Database::User::Weights::initialize_weights(std::vector<long double> temp)
{
    weights.location = temp[0];
    weights.cleanliness = temp[1];
    weights.staff = temp[2];
    weights.facilities = temp[3];
    weights.value_for_money = temp[4];
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
    this->sort_info = SortInfo();
    this->ratings = map_ratings();
    this->manual_weights = Weights();
    this->estimated_weights = Weights();
}

Database::User::~User()
{
    clean_filters_up();
    clean_reservations_up();
}

void Database::User::deposit(long double amount)
{
    wallet.amount += amount;
    wallet.transactions.push_front(wallet.amount);
}

Database::User::l_transactions Database::User::get_account_information(int num_transactions)
{
    l_transactions transactions;
    if(!wallet.transactions.empty())
    {
        for(auto transaction_itr = wallet.transactions.begin(); transaction_itr != wallet.transactions.end() && num_transactions > ConstNames::Minimum_No_Transactions; transaction_itr++, num_transactions--)
            transactions.push_back(*transaction_itr);
        return transactions;
    }
    return {0};
}

Database::l_hotels Database::User::get_hotels(Database::l_hotels& hotels)
{
    sort_hotels(hotels);
    if(could_apply_default_filter())
        apply_default_filter(hotels);
    if(filters.empty())
        return hotels;
    else{
        auto filter_itr = filters.begin();
        return get_filtered_hotels(hotels, filter_itr);
    }
}

void Database::User::apply_default_filter(Database::l_hotels& hotels)
{
    set_filters(FilterMode::DefaultAvgPrice);
    auto default_filter = filters[FilterMode::DefaultAvgPrice];
    if(default_filter->get_activity_status() == ConstNames::Active_Mode){
        default_filter->reset_filter();
        default_filter->filter(hotels);
        hotels = default_filter->get_hotels();
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

void Database::User::set_filters(const enum FilterMode mode)
{
    auto default_filter = filters.find(FilterMode::DefaultAvgPrice);
    if(default_filter == filters.end())
        filters[mode] = new DefaultAvgPriceFilter(booked_rooms);
    else
    {
        if(((*default_filter).second)->get_activity_status() == ConstNames::Active_Mode)
        {
            filters.erase(default_filter);
            filters[mode] = new DefaultAvgPriceFilter(booked_rooms);
        }
    }
}

void Database::User::set_rating(Hotel *hotel, Hotel::Rating *rating)
{
    ratings[hotel] = rating;
}

void Database::User::set_manual_weights(const Weights &manual_weights_obj)
{
    if(manual_weights_obj.activity == ConstNames::Inactive_Mode){
        this->manual_weights.activity = ConstNames::Inactive_Mode;
    }
    else
        this->manual_weights = manual_weights_obj;
}

void Database::User::set_default_filter(const bool activation_mode)
{
    if(activation_mode && could_apply_default_filter())
        set_filters(FilterMode::DefaultAvgPrice);
    else
        deactivate_default_filter();

}

void Database::User::deactivate_default_filter()
{
    auto default_filter = filters.find(FilterMode::DefaultAvgPrice);
    if(default_filter != filters.end())
        ((*default_filter).second)->deactivate();
}

Database::l_hotels Database::User::get_filtered_hotels(Database::l_hotels& hotels, m_filter::iterator& map_itr)
{
    if(map_itr == filters.end()){
        if(hotels.empty())
            throw Exception(ConstNames::Empty_msg);
        return hotels;
    }
    ((*map_itr).second)->reset_filter();
    ((*map_itr).second)->filter(hotels);
    auto new_hotels = ((*map_itr).second)->get_hotels();
    map_itr++;
    return get_filtered_hotels(new_hotels, map_itr);
}

bool Database::User::could_apply_default_filter()
{
    auto avg_filter = filters.find(FilterMode::AvgPrice);
    if(avg_filter != filters.end())
        return false;
    if(booked_rooms.size() < ConstNames::Minimum_Reservation_Count)
        return false;
    return true;
}

void Database::User::delete_filter()
{
    for(auto map_itr = filters.begin(); map_itr != filters.end(); map_itr++)
        delete (*map_itr).second;
    filters.clear();
}

bool Database::User::can_pay(long double cost)
{
    if(wallet.amount < cost)
        return false;
    return true;
}

Database::User::Weights Database::User::get_manual_weights() { return manual_weights;}

void Database::User::add_reserve_case(Hotel* hotel, Hotel::v_room& b_rooms, int check_in, int check_out, long double price)
{
    booked_rooms.push_front(new ReserveCase(hotel, b_rooms, price, check_in, check_out, id_reserve_list));
}

void Database::User::pay_cost(long double cost)
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
    hotels.sort([&] (Hotel* h1, Hotel* h2) -> bool
    {
        if(sort_info.property == Hotel::SortProperty::SP_RatingPersonal)
        {
            auto hotels_ratings = get_ratings_of_hotels(h1, h2);
            int cmp_res = Hotel::comparator(hotels_ratings.first, hotels_ratings.second);
            if(cmp_res) return (cmp_res == ConstNames::Smaller_cmp) ^ sort_info.mode;
        }
        else
        {
            int cmp_res = h1->comparator(sort_info.property, h2);
            if(cmp_res)    return (cmp_res == ConstNames::Smaller_cmp) ^ sort_info.mode;
        }
        return h1->id < h2->id;
    });
}

void Database::User::clean_filters_up()
{
    for(auto filter_itr = filters.begin(); filter_itr != filters.end(); filter_itr++)
    {
        delete (*filter_itr).second;
        (*filter_itr).second = nullptr;
    }
    filters.clear();
}

void Database::User::clean_reservations_up()
{
    for(auto book_itr = booked_rooms.begin(); book_itr != booked_rooms.end(); book_itr++)
    {
        delete *book_itr;
        *book_itr = nullptr;
    }
    booked_rooms.clear();
}

void Database::User::set_sort_info(const SortInfo& sort_info)
{
    this->sort_info = sort_info;
}

bool Database::User::the_default_filter_applied()
{
    auto default_filter = filters.find(FilterMode::DefaultAvgPrice);
    if(default_filter != filters.end())
        return filters[FilterMode::DefaultAvgPrice]->get_activity_status();
    return false;
}

pair_floats Database::User::get_ratings_of_hotels(Hotel* h1, Hotel* h2)
{
    if(manual_weights.activity)
        return make_pair(h1->calculate_avg_weighted(manual_weights.weights), h2->calculate_avg_weighted(manual_weights.weights));
    if(ratings.size() < ConstNames::Minimum_Ratings_Number)
        throw Exception(ConstNames::Insufficient_Ratings_msg);
    if(already_rated(h1) && already_rated(h2))
        return make_pair(get_overall_hotel_rating(h1), get_overall_hotel_rating(h2));
    if(already_rated(h1) || already_rated(h2))
    {
        auto pair = get_pair_of_hotels_and_ratings(h1, h2);
        auto weights = calculate_rating();
        return make_pair(pair.first, (pair.second)->calculate_avg_weighted(weights.weights));
    }
    auto i_weights = calculate_rating();
    auto j_weights = calculate_rating();
    return make_pair(h1->calculate_avg_weighted(i_weights.weights), h2->calculate_avg_weighted(j_weights.weights));
}

bool Database::User::already_rated(Hotel *hotel)
{
    auto searched_hotel = ratings.find(hotel);
    if(searched_hotel != ratings.end())
        return true;
    return false;
}

long double Database::User::get_overall_hotel_rating(Hotel *hotel)
{
    auto rating = ratings.find(hotel);
    return (*rating).second->get_overall_rating();
}

std::pair<long double, Database::Hotel*> Database::User::get_pair_of_hotels_and_ratings(Database::Hotel* h1, Database::Hotel* h2)
{
    if(already_rated(h1) && !already_rated(h2))
        return make_pair(get_overall_hotel_rating(h1), h2);
    return make_pair(get_overall_hotel_rating(h2), h1);
}

Database::User::Weights Database::User::calculate_rating()
{
    if(this->estimated_weights == Weights())
    {
        // cerr << "First Time\n";
        auto backend = Backend::get_instance();
        this->estimated_weights = backend->calculate_weights(this, get_ratings());
        if(estimated_weights == Weights())
            cerr << "RIDI: " << estimated_weights.weights.location << endl;
        return estimated_weights;
    }
    return estimated_weights;
}

Database::Hotel::v_rating Database::User::get_ratings()
{
    auto temp = Hotel::v_rating();
    for(auto rating_itr = ratings.begin(); rating_itr != ratings.end(); rating_itr++)
        temp.push_back((*rating_itr).second);
    return temp;
}
