#include "API.hpp"

API* API::instance = nullptr;

API* API::get_instance()
{
    if(instance == nullptr)
        instance = new API();
    return instance;
}

void API::release()
{
    delete instance;
    instance = nullptr;
}

API::API()
{
    object_relational = ObjectRelational::get_instance();
    backend = Backend::get_instance();
}

API::~API() {}

void API::database_setup(Backend::data_t data)
{
    object_relational->database_setup(data);
}

Backend::data_t API::parse_content(Content content, char separator)
{
    return backend->parse_content(content, separator);
}

Content API::command_processor(Backend::data_t command_data)
{
    return backend->command_processor(command_data);
}
