#ifndef _USER_H
#define _USER_H

#include "ReserveCase.hpp"

class Database::User
{
public:
    class Filter;
    typedef std::list<float> v_transactions;
    typedef std::list<ReserveCase*> v_booked;
    typedef std::list<Filter*> l_filter;
    friend class Database;

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

private:
    User(const UserInfo& info);
    std::string generate_id();
    void deposit(float amount);
    info_t get_account_information(int num_transactions);
    info_t get_transactions_info(int num_transactions);
    info_t get_hotels(Database::l_hotels& hotels);
    void sort_hotels(Database::l_hotels& hotels);
    info_t get_hotels_info(Database::l_hotels& hotels);

private:
    std::string id;
    std::string username;
    std::string email;
    long long password;
    Wallet wallet;
    v_booked booked_rooms;
    l_filter filters;
};

#endif
