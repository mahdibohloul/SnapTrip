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
    friend class Database;
    typedef std::map<Room_Class, std::string> m_room_to_string;
    static m_room_to_string room_string;
    t_id get_id();

public:
    virtual ~Room();
    static void reset_room_id(v_room& rooms);
    virtual void reset_id() = 0;
    virtual bool can_reserve(int check_in, int check_out) const;
    virtual void delete_reserve(int check_in, int check_out);

protected:
    Room(std::string id, enum Room_Class type, long double price);
    virtual std::string generate_id() = 0;
    virtual bool overlap(ReserveRoom reserve, int check_in, int check_out) const;
    virtual void booked(int check_in, int check_out);

protected:
    Room_Class type;
    long double price;
    t_id id;
    v_reserve reserves;
};

#endif
