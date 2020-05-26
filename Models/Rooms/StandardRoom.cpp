#include "StandardRoom.hpp"
#include "../../ConstNames.hpp"
#include <sstream>
using namespace std;

int StandardRoom::current_id = ConstNames::First_Room_Id;

StandardRoom::StandardRoom(float price) : Database::Hotel::Room(generate_id(), Room_Class::Standard, price){}

StandardRoom::~StandardRoom(){}

string StandardRoom::generate_id()
{
    ostringstream os;
    os << ConstNames::Standard_Room_Prefix << current_id;
    current_id++;
    return os.str();
}

void StandardRoom::reset_id() { current_id = ConstNames::First_Room_Id; }
