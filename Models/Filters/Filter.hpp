#ifndef _FILTER_H
#define _FILTER_H

#include "../User/User.hpp"

class Database::User::Filter
{
public:
    typedef std::list<Database::Hotel*> v_hotels;
public:
    virtual ~Filter();

private:
    Filter();

private:
    v_hotels hotels;
};

#endif
