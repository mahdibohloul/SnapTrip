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
    virtual void deactivate();
    virtual void activate();
    virtual bool get_activity_status();

protected:
    Filter(bool activation_mode = ConstNames::Active_Mode);

protected:
    Database::l_hotels hotels;
    bool activity;
};

#endif
