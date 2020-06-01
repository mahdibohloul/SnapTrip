#ifndef _COMMENT_H
#define _COMMENT_H

#include "User/User.hpp"


class Database::Comment
{
public:
    friend class Database;

public:
    ~Comment();
    info_t get_writer();
    info_t get_comment();

private:
    Comment(std::string comment, User* user, Hotel* hotel);

private:
    std::string comment;
    User* user;
    Hotel* hotel;
};


#endif


