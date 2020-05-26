#ifndef _ROOM_H
#define _ROOM_H

#include "../Hotel.hpp"
#include "../../ConstNames.hpp"

class Database::Hotel::Room
{
public:
    enum Room_Class {
        Standard,
        Deluxe,
        Luxury,
        Premium
    };

public:
    virtual ~Room();
    static void reset_room_id(v_room& rooms);
    virtual void reset_id() = 0;
    friend class Hotel;

protected:
    Room(std::string id, enum Room_Class type, float price);
    virtual std::string generate_id() = 0;
private:
    Room_Class type;
    float price;
    t_id id;
    bool booked;
};

#endif
