#ifndef _COMMENT_H
#define _COMMENT_H

#include "User/User.hpp"


class Database::Comment
{
public:
    ~Comment();

private:
    Comment();

private:
    std::string comment;
    User* user;
    Hotel* hotel;
};


#endif
