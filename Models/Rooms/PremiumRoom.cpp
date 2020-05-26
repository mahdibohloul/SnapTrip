#include "PremiumRoom.hpp"
#include "../../ConstNames.hpp"
#include <sstream>
using namespace std;

int PremiumRoom::current_id = ConstNames::First_Room_Id;

PremiumRoom::PremiumRoom(float price) : Database::Hotel::Room(generate_id(), Room_Class::Premium, price) {}

PremiumRoom::~PremiumRoom() {}

string PremiumRoom::generate_id()
{
    ostringstream os;
    os << ConstNames::Premium_Room_Prefix << current_id;
    current_id++;
    return os.str();
}

void PremiumRoom::reset_id() {current_id = ConstNames::First_Room_Id;}
