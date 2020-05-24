#ifndef _RESERVECASE_H
#define _RESERVECASE_H

#include "../Rooms/Room.hpp"
#include <list>

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
};

#endif
