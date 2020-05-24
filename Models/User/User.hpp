#ifndef _USER_H
#define _USER_H

#include "ReserveCase.hpp"

class Database::User
{
public:
    typedef std::list<float> v_transactions;
    typedef std::list<ReserveCase*> v_booked;
    class Filter;

private:
    static int current_id;
    struct Wallet
    {
        float amount;
        v_transactions transactions;
    };

public:
    ~User();

private:
    User();

private:
    std::string id;
    std::string username;
    std::string email;
    long long password;
    v_booked booked_rooms;
};

#endif
