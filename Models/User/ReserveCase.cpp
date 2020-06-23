#include "ReserveCase.hpp"
#include "../../ConstNames.hpp"
using namespace std;


Database::User::ReserveCase::ReserveCase(Database::Hotel* hotel, v_rooms& b_rooms, long double price, int check_in, int check_out, list<int>& l_id)
{
    this->id = this->generate_id(l_id);
    this->hotel = hotel;
    this->rooms = b_rooms;
    this->cost = price;
    this->check_in = check_in;
    this->check_out = check_out;
}

Database::User::ReserveCase::~ReserveCase()
{
    for(auto room_itr = rooms.begin(); room_itr != rooms.end(); room_itr++)
        (*room_itr)->delete_reserve(check_in, check_out);
    rooms.clear();
}


int Database::User::ReserveCase::generate_id(std::list<int> &l_id)
{
    if(l_id.empty())
    {
        l_id.push_front(ConstNames::First_Reserve_Id);
        return *l_id.begin();
    }
    auto id = l_id.begin();
    l_id.push_front(*id + 1);
    return *l_id.begin();
}

int Database::User::ReserveCase::get_reserve_id() { return id; }

string Database::User::ReserveCase::get_hotel_reserved_id() { return hotel->id; }

int Database::User::ReserveCase::get_quantity_of_reserves() { return rooms.size(); }

long double Database::User::ReserveCase::get_cost_of_reserves() { return cost; }

pair<int, int> Database::User::ReserveCase::get_reserved_days() { return make_pair(check_in, check_out); }

string Database::User::ReserveCase::get_type_of_reserved_rooms() { return Database::Hotel::Room::room_string[rooms[0]->type]; }

int Database::User::ReserveCase::get_residency_period() { return check_out - check_in; }
