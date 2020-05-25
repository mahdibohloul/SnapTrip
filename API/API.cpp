#include "API.hpp"

API::API()
{
    object_relational = ObjectRelational::get_instance();
    backend = Backend::get_instance();
}

void API::database_setup(Backend::data_t data)
{
    object_relational->database_setup(data);
}

Backend::data_t API::parse_content(Content content, char separator)
{
    return backend->parse_content(content, separator);
}
