#ifndef _RATING_H
#define _RATING_H

#include "User/User.hpp"

typedef std::vector<long double> v_double;

class Database::Hotel::Rating
{
public:
    friend class Hotel;

public:
    ~Rating();
    long double get_overall_rating();
    long double get_sum_weighted(v_double weights);

private:
    Rating(const RatingInfo& rating_info);

private:
    long double location;
    long double cleanliness;
    long double staff;
    long double facilities;
    long double value_for_money;
    long double overall_rating;
};

#endif
