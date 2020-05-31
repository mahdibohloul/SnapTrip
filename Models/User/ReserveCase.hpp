#ifndef _RESERVECASE_H
#define _RESERVECASE_H

#include "User.hpp"
#include <list>

class Database::User::ReserveCase
{
public:
    typedef std::vector<Database::Hotel::Room*> v_rooms;
    friend class User;
    friend class Database;

public:
    ~ReserveCase();

private:
    ReserveCase(Database::Hotel* hotel, v_rooms& b_rooms, float price, int check_in, int check_out, std::list<int>& l_id);
    int generate_id(std::list<int>& l_id);

private:
    int id;
    Database::Hotel* hotel;
    v_rooms rooms;
    float cost;
    int check_in;
    int check_out;
};

#endif
