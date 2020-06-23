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
    long double calculate_average(const Database::User::l_booked& booked_rooms);
    long double calculate_average_deviation(const Database::User::l_booked& booked_rooms);

    bool in_range(long double hotel_avg_price);

private:
    long double average_deviation;
    long double average;

};

#endif
