#ifndef _RESERVECASE_H
#define _RESERVECASE_H

#include "../Rooms/Room.hpp"
#include <list>

typedef int day_t;

class ReserveCase
{
public:
    typedef std::list<Database::Hotel::Room*> v_rooms;

public:
    ~ReserveCase();

private:
    ReserveCase();

private:
    static int current_id;
    t_id id;
    Database::Hotel* hotel;
    v_rooms rooms;
    float cost;
    day_t check_in;
    day_t check_out;
};

#endif
