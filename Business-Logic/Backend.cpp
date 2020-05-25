#include "Backend.hpp"
#include "../ORM/ObjectRelational.hpp"
using namespace std;

Backend* Backend::instance = nullptr;

Backend* Backend::get_instance()
{
    if(instance == nullptr){
        instance = new Backend();
    }
    return instance;
}

void Backend::release()
{
    delete instance;
    instance = nullptr;
}

Backend::Backend()
{
    object_relational = nullptr;
}

Backend::~Backend() {}


Backend::data_t Backend::parse_content(Content content, char separator)
{
    data_t parsed_data;
    int separator_pos;
    while((separator_pos = content.find(separator)) != string::npos)
    {
        parsed_data.push_back(content.substr(0, separator_pos));
        content = content.substr(separator_pos + 1);
    }
    parsed_data.push_back(content.substr(separator_pos + 1));
    return parsed_data;
}
