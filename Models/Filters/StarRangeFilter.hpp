#ifndef _STARRANGEFILTER_H
#define _STARRANGEFILTER_H

#include "Filter.hpp"

class StarRangeFilter : public Database::User::Filter
{
public:
    StarRangeFilter(const Database::User::FilterInfo&);
    ~StarRangeFilter();
    void filter(Database::l_hotels&);

private:
    bool in_range(const Database::Hotel* hotel);

private:
     int min_star;
     int max_star;
};

#endif
