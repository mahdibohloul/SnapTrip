#ifndef _PREMIUMROOM_H
#define _PREMIUMROOM_H

#include "Room.hpp"

class PremiumRoom : public Database::Hotel::Room
{
public:
    PremiumRoom(float price);
    ~PremiumRoom();
    std::string generate_id();

private:
    static int current_id;
};
#endif
