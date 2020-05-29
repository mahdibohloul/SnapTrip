#ifndef _CITYFILTER_H
#define _CITYFILTER_H

#include "Filter.hpp"

class CityFilter : public Database::User::Filter
{
public:
    CityFilter(const Database::User::FilterInfo&);
    ~CityFilter();
    void filter(Database::l_hotels&);
    std::string get_city() { return city; }

private:
    std::string city;

};
#endif
