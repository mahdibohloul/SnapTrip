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

long double Database::Hotel::Rating::get_overall_rating() { return overall_rating; }

long double Database::Hotel::Rating::get_sum_weighted(v_double weights)
{
    long double sum = 0;
    sum += location * weights[0]; sum += cleanliness * weights[1]; sum += staff * weights[2]; sum += facilities * weights[3]; sum += value_for_money * weights[4];
    return sum;
}
