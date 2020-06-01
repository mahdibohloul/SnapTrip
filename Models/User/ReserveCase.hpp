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
    int get_reserve_id();
    std::string get_hotel_reserved_id();
    int get_quantity_of_reserves();
    float get_cost_of_reserves();
    std::pair<int,int> get_reserved_days();
    std::string get_type_of_reserved_rooms();

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
