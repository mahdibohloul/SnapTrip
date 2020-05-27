#ifndef _USER_H
#define _USER_H

#include "ReserveCase.hpp"

class Database::User
{
public:
    typedef std::list<float> v_transactions;
    typedef std::list<ReserveCase*> v_booked;
    class Filter;
    friend class Database;

private:
    static int current_id;
    struct Wallet
    {
        float amount;
        v_transactions transactions;
        Wallet(float i_amount = 0.0f, v_transactions i_transactions = v_transactions())
        {
            amount = i_amount;
            transactions = i_transactions;
        }
    };

public:
    ~User();

private:
    User(const UserInfo& info);
    std::string generate_id();

private:
    std::string id;
    std::string username;
    std::string email;
    long long password;
    Wallet wallet;
    v_booked booked_rooms;
};

#endif
