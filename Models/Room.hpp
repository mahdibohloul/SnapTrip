#ifndef _ROOM_H
#define _ROOM_H

#include "Hotel.hpp"
#include "../ConstNames.hpp"

class Database::Hotel::Room
{
public:
    enum Room_Class {
        Standard,
        Delux,
        Luxury,
        Premium
    };

protected:
    Room(std::string id, enum Room_Class type, float price);
    virtual ~Room();
    virtual std::string generate_id() = 0;
private:
    Room_Class type;
    float price;
    std::string id;
    bool booked;
};

#endif
