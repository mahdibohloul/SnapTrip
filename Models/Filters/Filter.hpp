#ifndef _FILTER_H
#define _FILTER_H

#include "../User/User.hpp"

class Database::User::Filter
{
public:
    virtual ~Filter();
    virtual void filter(Database::l_hotels&) = 0;
    virtual Database::l_hotels get_hotels();
    virtual void reset_filter();

protected:
    Filter();

protected:
    Database::l_hotels hotels;
};

#endif
