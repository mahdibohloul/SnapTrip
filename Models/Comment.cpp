#include "Comment.hpp"
#include <sstream>
using namespace std;


Database::Comment::Comment(string comment, User* user, Hotel* hotel)
{
    this->comment = comment;
    this->user = user;
    this->hotel = hotel;
}

Database::Comment::~Comment() {}

info_t Database::Comment::get_info()
{
    ostringstream os;
    os << user->username << ConstNames::Colon << ConstNames::Space << comment;
    return os.str();
}
