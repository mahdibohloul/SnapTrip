#ifndef _DELUXEROOM_H
#define _DELUXEROOM_H

#include "Room.hpp"

class DeluxeRoom : public Database::Hotel::Room
{
public:
    DeluxeRoom(float price);
    ~DeluxeRoom();
    std::string generate_id();

private:
    static int current_id;

};

#endif
