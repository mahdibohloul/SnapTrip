#include "Room.hpp"


Database::Hotel::Room::Room(std::string id, enum Room_Class type, float price)
{
    this->id = id;
    this->type = type;
    this->price = price;
    this->booked = ConstNames::Not_Booked;
}

Database::Hotel::Room::~Room() {}
