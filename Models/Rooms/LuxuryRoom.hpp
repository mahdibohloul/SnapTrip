#ifndef _LUXURYROOM_H
#define _LUXURYROOM_H

#include "Room.hpp"

class LuxuryRoom : public Database::Hotel::Room
{
public:
    LuxuryRoom(float price);
    ~LuxuryRoom();
    std::string generate_id();
    void reset_id();

private:
    static int current_id;

};


#endif
