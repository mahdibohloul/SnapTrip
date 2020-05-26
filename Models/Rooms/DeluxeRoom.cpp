#include "DeluxeRoom.hpp"
#include "../../ConstNames.hpp"
#include <sstream>
using namespace std;

int DeluxeRoom::current_id = ConstNames::First_Room_Id;

DeluxeRoom::DeluxeRoom(float price) : Database::Hotel::Room(generate_id(), Room_Class::Deluxe, price) {}

DeluxeRoom::~DeluxeRoom() {}

string DeluxeRoom::generate_id()
{
    ostringstream os;
    os << ConstNames::Delux_Room_Prefix << current_id;
    current_id++;
    return os.str();
}

void DeluxeRoom::reset_id() {current_id = ConstNames::First_Room_Id;}
