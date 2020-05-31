#include "Rating.hpp"
using namespace std;

Database::Hotel::Rating::Rating(const RatingInfo& rating_info)
{
    this->location = rating_info.location;
    this->cleanliness = rating_info.cleanliness;
    this->staff = rating_info.staff;
    this->facilities = rating_info.facilities;
    this->value_for_money = rating_info.value_for_money;
    this->overall_rating = rating_info.overall_rating;
}

Database::Hotel::Rating::~Rating() {}
