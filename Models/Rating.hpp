#ifndef _RATING_H
#define _RATING_H

#include "User/User.hpp"

class Database::Hotel::Rating
{
public:
    friend class Hotel;

public:
    ~Rating();

private:
    Rating(const RatingInfo& rating_info);

private:
    float location;
    float cleanliness;
    float staff;
    float facilities;
    float value_for_money;
    float overall_rating;
};

#endif
