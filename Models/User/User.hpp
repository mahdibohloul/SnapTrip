#ifndef _USER_H
#define _USER_H

#include "../Rooms/Room.hpp"

typedef std::vector<long double> v_double;
typedef std::pair<long double, long double> pair_floats;

class Database::User
{
public:
    friend class Database;
    enum FilterMode
    {
        DefaultAvgPrice,
        City,
        StarRange,
        AvgPrice,
        Advanced
    };
    enum SortMode
    {
        SM_Ascending,
        SM_Descending
    };
    struct FilterInfo
    {
        std::map<std::string, Hotel::Room::Room_Class> m_room_type;
        FilterMode mode;
        std::string city;
        int min_star;
        int max_star;
        long double min_price;
        long double max_price;
        Hotel::Room::Room_Class type;
        int quantity;
        int check_in;
        int check_out;
        FilterInfo();
    };
    struct ReserveInfo
    {
        std::map<std::string, Hotel::Room::Room_Class> m_room_type;
        std::string hotel_id;
        Hotel::Room::Room_Class type;
        int quantity;
        int check_in;
        int check_out;
        ReserveInfo();
    };
    struct SortInfo
    {
        std::map<std::string, Hotel::SortProperty> map_sort_property;
        Hotel::SortProperty property;
        SortMode mode;
        SortInfo();
    };
    enum WeightsEnum
    {
        WE_Location,
        WE_Cleanliness,
        WE_Staff,
        WE_Facilities,
        WE_Value
    };
    struct Weights
    {
        Hotel::RatingInfo weights;
        bool activity;
        Weights(const Hotel::RatingInfo& i_weights = Hotel::RatingInfo(), bool i_activity = ConstNames::Inactive_Mode)
        {
            weights = i_weights;
            activity = i_activity;
        }
        bool operator==(const Weights& second_weights);
        long double operator[](const int which);
        void initialize_weights(const v_double i_weights);
    };
    class Filter;
    class ReserveCase;
    typedef std::list<long double> l_transactions;
    typedef std::list<ReserveCase*> l_booked;
    typedef std::map<FilterMode, Filter*> m_filter;
    typedef std::map<Hotel*, Hotel::Rating*> map_ratings;

private:
    struct Wallet
    {
        long double amount;
        l_transactions transactions;
        Wallet(long double i_amount = 0.0f)
        {
            amount = i_amount;
            transactions.push_front(amount);
        }
    };
    static int current_id;

public:
    ~User();

    void set_filters(const FilterInfo& filter_info);
    void set_default_filter(const bool activation_mode);
    void set_sort_info(const SortInfo& sort_info);
    void set_rating(Hotel* hotel, Hotel::Rating* rating);
    void set_manual_weights(const Weights& manual_weights);
    void delete_filter();
    void deactivate_default_filter();
    void delete_reserve(int id);
    Weights get_manual_weights();
    Weights get_estimated_weights();

    bool can_pay(long double cost);
    bool the_default_filter_applied();
    bool already_rated(Hotel* hotel);

    long double get_overall_hotel_rating(Hotel* hotel);

private:
    User(const UserInfo& info);

    info_t generate_id();
    l_transactions get_account_information(int num_transactions);
    Database::l_hotels get_hotels(Database::l_hotels& hotels);
    Database::l_hotels get_filtered_hotels(Database::l_hotels& hotels, m_filter::iterator& map_itr);

    void sort_hotels(Database::l_hotels& hotels);
    void apply_default_filter(Database::l_hotels& hotels);
    void deposit(long double amount);
    void set_filters(const enum FilterMode mode);
    void add_reserve_case(Hotel* hotel, Hotel::v_room& b_rooms, int check_in, int check_out, long double price);
    void pay_cost(long double cost);
    void delete_from_reserves(ReserveCase* reserve);
    void clean_filters_up();
    void clean_reservations_up();

    pair_floats get_ratings_of_hotels(Hotel* h1, Hotel* h2);
    float get_rating_of_hotel(Hotel* hotel, Weights weights);

    Weights calculate_rating();
    long double calculate_overall_rating(Weights weights);

    ReserveCase* query_in_reserves(int id);

    l_booked get_reserve();

    bool could_apply_default_filter();

    Hotel::v_rating get_ratings();

private:
    info_t id;
    info_t username;
    info_t email;
    long long password;
    Wallet wallet;
    SortInfo sort_info;
    l_booked booked_rooms;
    map_ratings ratings;
    m_filter filters;
    std::list<int> id_reserve_list;
    Weights manual_weights;
    Weights estimated_weights;
};

#endif
