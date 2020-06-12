#ifndef _DEFAULTAVGPRICEFILTER_H
#define _DEFAULTAVGPRICEFILTER_H


#include "Filter.hpp"

class DefaultAvgPriceFilter : public Database::User::Filter
{
public:
    DefaultAvgPriceFilter(const Database::User::l_booked& booked_rooms);
    ~DefaultAvgPriceFilter();
    void filter(Database::l_hotels&);

private:
    float calculate_average(const Database::User::l_booked& booked_rooms);
    float calculate_average_deviation(const Database::User::l_booked& booked_rooms);

    bool in_range(float hotel_avg_price);

private:
    float average_deviation;
    float average;

};

#endif
