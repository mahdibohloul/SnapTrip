#include "LuxuryRoom.hpp"
#include "../../ConstNames.hpp"
#include <sstream>
using namespace std;

int LuxuryRoom::current_id = ConstNames::First_Room_Id;

LuxuryRoom::LuxuryRoom(float price) : Database::Hotel::Room(generate_id(), Room_Class::Luxury, price) {}

LuxuryRoom::~LuxuryRoom() {}

string LuxuryRoom::generate_id()
{
    ostringstream os;
    os << ConstNames::Luxury_Room_Prefix << current_id;
    current_id++;
    return os.str();
}

void LuxuryRoom::reset_id() {current_id = ConstNames::First_Room_Id;}
