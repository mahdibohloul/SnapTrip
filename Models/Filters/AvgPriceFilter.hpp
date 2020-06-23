#ifndef _AVGPRICEFILTER_H
#define _AVGPRICEFILTER_H

#include "Filter.hpp"

class AvgPriceFilter : public Database::User::Filter
{
public:
    AvgPriceFilter(const Database::User::FilterInfo&);
    ~AvgPriceFilter();
    void filter(Database::l_hotels&);

private:
    bool in_range(const Database::Hotel* hotel);

private:
    long double min_price;
    long double max_price;

};

#endif
