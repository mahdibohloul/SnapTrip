#ifndef _API_H
#define _API_H

#include "../ORM/ObjectRelational.hpp"
#include "../Business-Logic/Backend.hpp"

class API
{
public:
    API();
    ~API();
    void database_setup(Backend::data_t data);
    Backend::data_t parse_content(Content content, char separator);

private:
    ObjectRelational* object_relational;
    Backend* backend;


};
#endif
