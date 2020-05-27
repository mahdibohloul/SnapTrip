#include "User.hpp"
#include "sstream"
using namespace std;

int Database::User::current_id = ConstNames::First_User_Id;

Database::User::User(const UserInfo& info)
{
    this->id = generate_id();
    this->username = info.username;
    this->email = info.email;
    this->password = info.password;
    wallet = Wallet();
    booked_rooms = v_booked();
}

string Database::User::generate_id()
{
    ostringstream os;
    os << ConstNames::User_Id_Prefix << current_id;
    current_id++;
    return os.str();
}
