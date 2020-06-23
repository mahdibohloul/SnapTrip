#ifndef _PREMIUMROOM_H
#define _PREMIUMROOM_H

#include "Room.hpp"

class PremiumRoom : public Database::Hotel::Room
{
public:
    PremiumRoom(long double price);
    ~PremiumRoom();
    std::string generate_id();
    void reset_id();

private:
    static int current_id;
};
#endif
