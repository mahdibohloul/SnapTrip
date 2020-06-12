#ifndef _API_H
#define _API_H

#include "../ORM/ObjectRelational.hpp"

class API
{
public:
    class Result;

public:
    static API* get_instance();
    static void release();
    ~API();
    void database_setup(Backend::data_t data);
    Backend::data_t parse_content(Content content, char separator);
    void command_processor(Backend::data_t command_data);
    template<class T> void responding(T response);
    std::string get_response();

private:
    API();

private:
    static API* instance;
    ObjectRelational* object_relational;
    Backend* backend;
    Result* result;


};
#endif
