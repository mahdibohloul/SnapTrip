#ifndef _ADVANCEDFILTER_H
#define _ADVANCEDFILTER_H

#include "Filter.hpp"

class AdvancedFilter : public Database::User::Filter
{
public:
    AdvancedFilter(const Database::User::FilterInfo& filter_info);
    ~AdvancedFilter();
    void filter(Database::l_hotels&);

private:
    bool in_range(const Database::Hotel* hotel);

private:
    Database::Hotel::Room::Room_Class type;
    int quantity;
    int check_in;
    int check_out;
};
#endif
