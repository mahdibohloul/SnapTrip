#include "Filter.hpp"


Database::User::Filter::Filter(bool activation_mode) : hotels(Database::l_hotels())
{
    activity = activation_mode;
}

Database::User::Filter::~Filter()
{
    hotels.clear();
}

Database::l_hotels Database::User::Filter::get_hotels() { return hotels; }

void Database::User::Filter::reset_filter() { hotels.clear(); }

void Database::User::Filter::deactivate() { activity = ConstNames::Inactive_Mode; }

void Database::User::Filter::activate() { activity = ConstNames::Active_Mode; }

bool Database::User::Filter::get_activity_status() { return activity; }
