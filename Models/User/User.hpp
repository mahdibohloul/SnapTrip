#ifndef _USER_H
#define _USER_H

#include "ReserveCase.hpp"

class Database::User
{
public:
    enum FilterMode
    {
        City,
        StarRange,
        AvgPrice,
        Advanced
    };
    class Filter;
    typedef std::list<float> v_transactions;
    typedef std::list<ReserveCase*> v_booked;
    typedef std::map<FilterMode, Filter*> m_filter;
    friend class Database;
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
        FilterInfo()
        {
            m_room_type.insert(make_pair(ConstNames::StandardRoom, Hotel::Room::Room_Class::Standard));
            m_room_type.insert(make_pair(ConstNames::DeluxeRoom, Hotel::Room::Room_Class::Deluxe));
            m_room_type.insert(make_pair(ConstNames::LuxuryRoom, Hotel::Room::Room_Class::Luxury));
            m_room_type.insert(make_pair(ConstNames::PremiumRoom, Hotel::Room::Room_Class::Premium));
            min_star = 0;
            max_star = 0;
            min_price = -1;
            max_price = -1;
            quantity = -1;
            check_in = 0;
            check_out = 0;
            type = Hotel::Room::Room_Class::None;
        }
    };

private:
    static int current_id;
    struct Wallet
    {
        float amount;
        v_transactions transactions;
        Wallet(float i_amount = 0.0f)
        {
            amount = i_amount;
            transactions.push_front(amount);
        }
    };

public:
    ~User();
    void set_filters(const FilterInfo& filter_info);
    void delete_filter();

private:
    User(const UserInfo& info);
    std::string generate_id();
    void deposit(float amount);
    info_t get_account_information(int num_transactions);
    info_t get_transactions_info(int num_transactions);
    info_t get_hotels(Database::l_hotels& hotels);
    void sort_hotels(Database::l_hotels& hotels);
    info_t get_hotels_info(Database::l_hotels& hotels);
    info_t get_filtered_hotels(Database::l_hotels& hotels, m_filter::iterator& map_itr);
    void set_filter(const FilterInfo& filter_info);

private:
    std::string id;
    std::string username;
    std::string email;
    long long password;
    Wallet wallet;
    v_booked booked_rooms;
    m_filter filters;
};

#endif
