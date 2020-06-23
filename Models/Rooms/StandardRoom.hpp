#ifndef _STANDARDROOM_H
#define _STANDARDROOM_H

#include "Room.hpp"

class StandardRoom : public Database::Hotel::Room
{
public:
    StandardRoom(long double price);
    ~StandardRoom();
    std::string generate_id();
    void reset_id();

private:
    static int current_id;
};

#endif
