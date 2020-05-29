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
        Premium,
        None
    };
    struct ReserveRoom
    {
        int check_in;
        int check_out;
        ReserveRoom(int in = 0, int out = 0)
        {
            check_in = in;
            check_out = out;
        }
    };

public:
    typedef std::vector<ReserveRoom> v_reserve;
    friend class Hotel;

public:
    virtual ~Room();
    static void reset_room_id(v_room& rooms);
    virtual void reset_id() = 0;
    virtual bool can_reserve(int check_in, int check_out) const;

protected:
    Room(std::string id, enum Room_Class type, float price);
    virtual std::string generate_id() = 0;
    virtual bool overlap(ReserveRoom reserve, int check_in, int check_out) const;

protected:
    Room_Class type;
    float price;
    t_id id;
    v_reserve reserves;
};

#endif
