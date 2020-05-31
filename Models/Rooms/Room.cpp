#include "Room.hpp"
using namespace std;

Database::Hotel::Room::m_room_to_string Database::Hotel::Room::room_string {
    make_pair(Room_Class::Standard, ConstNames::StandardRoom),
    make_pair(Room_Class::Deluxe, ConstNames::DeluxeRoom),
    make_pair(Room_Class::Luxury, ConstNames::LuxuryRoom),
    make_pair(Room_Class::Premium, ConstNames::PremiumRoom)
};

Database::Hotel::Room::Room(std::string id, enum Room_Class type, float price)
{
    this->id = id;
    this->type = type;
    this->price = price;
    this->reserves = v_reserve();
}

Database::Hotel::Room::~Room() {}


void Database::Hotel::Room::reset_room_id(v_room &rooms)
{
    for(auto room = rooms.begin(); room != rooms.end(); room++)
        (*room)->reset_id();
}

bool Database::Hotel::Room::can_reserve(int check_in, int check_out) const
{
    for(auto reserve_itr = reserves.begin(); reserve_itr != reserves.end(); reserve_itr++)
    {
        if(overlap(*reserve_itr, check_in, check_out) == ConstNames::Overlapping)
            return false;
    }
    return true;
}

bool Database::Hotel::Room::overlap(Database::Hotel::Room::ReserveRoom reserve, int check_in, int check_out) const
{
    if(reserve.check_out < check_in && reserve.check_in > check_out)
        return !ConstNames::Overlapping;
    return ConstNames::Overlapping;
}

void Database::Hotel::Room::booked(int check_in, int check_out)
{
    reserves.push_back(ReserveRoom(check_in, check_out));
}

void Database::Hotel::Room::delete_reserve(int check_in, int check_out)
{
    auto want_delete = reserves.begin();
    for(auto reserve_itr = reserves.begin(); reserve_itr != reserves.end(); reserve_itr++)
    {
        if((*reserve_itr).check_in == check_in && (*reserve_itr).check_out == check_out)
        {
            want_delete = reserve_itr;
            break;
        }
    }
    reserves.erase(want_delete);
}
