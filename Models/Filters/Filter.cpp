#include "Filter.hpp"


Database::User::Filter::Filter() : hotels(Database::l_hotels()) {}

Database::User::Filter::~Filter()
{
    hotels.clear();
}

Database::l_hotels Database::User::Filter::get_hotels() { return hotels; }

void Database::User::Filter::reset_filter() { hotels.clear(); }
