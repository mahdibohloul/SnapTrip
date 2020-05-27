#ifndef _API_H
#define _API_H

#include "../ORM/ObjectRelational.hpp"
#include "../Business-Logic/Backend.hpp"

class API
{
public:
    static API* get_instance();
    static void release();
    ~API();
    void database_setup(Backend::data_t data);
    Backend::data_t parse_content(Content content, char separator);
    Content command_processor(Backend::data_t command_data);

private:
    API();

private:
    static API* instance;
    ObjectRelational* object_relational;
    Backend* backend;


};
#endif
