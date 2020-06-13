#ifndef _USER_H
#define _USER_H

#include "../Rooms/Room.hpp"

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
        float min_price;
        float max_price;
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
    class Filter;
    class ReserveCase;
    typedef std::list<float> l_transactions;
    typedef std::list<ReserveCase*> l_booked;
    typedef std::map<FilterMode, Filter*> m_filter;

private:
    struct Wallet
    {
        float amount;
        l_transactions transactions;
        Wallet(float i_amount = 0.0f)
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
    void delete_filter();
    void deactivate_default_filter();
    void delete_reserve(int id);

    bool can_pay(float cost);
    bool the_default_filter_applied();

private:
    User(const UserInfo& info);

    info_t generate_id();
    l_transactions get_account_information(int num_transactions);
    Database::l_hotels get_hotels(Database::l_hotels& hotels);
    Database::l_hotels get_filtered_hotels(Database::l_hotels& hotels, m_filter::iterator& map_itr);

    void sort_hotels(Database::l_hotels& hotels);
    void apply_default_filter(Database::l_hotels& hotels);
    void deposit(float amount);
    void set_filters(const enum FilterMode mode);
    void add_reserve_case(Hotel* hotel, Hotel::v_room& b_rooms, int check_in, int check_out, float price);
    void pay_cost(float cost);
    void delete_from_reserves(ReserveCase* reserve);
    void clean_filters_up();
    void clean_reservations_up();

    ReserveCase* query_in_reserves(int id);

    l_booked get_reserve();

    bool could_apply_default_filter();

private:
    info_t id;
    info_t username;
    info_t email;
    long long password;
    Wallet wallet;
    SortInfo sort_info;
    l_booked booked_rooms;
    m_filter filters;
    std::list<int> id_reserve_list;
};

#endif
