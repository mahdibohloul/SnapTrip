#ifndef _AVGPRICEFILTER_H
#define _AVGPRICEFILTER_H

#include "Filter.hpp"

class AvgPriceFilter : public Database::User::Filter
{
public:
    AvgPriceFilter();
    ~AvgPriceFilter();

};

#endif
