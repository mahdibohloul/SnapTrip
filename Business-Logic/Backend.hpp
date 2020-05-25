#ifndef _BACKEND_H
#define _BACKEND_H

#include <vector>
#include <string>

typedef std::string Content;

class ObjectRelational;

class Backend
{
public:
    typedef std::vector<std::string> data_t;

public:
    ~Backend();
    static Backend* get_instance();
    static void release();
    data_t parse_content(Content content, char separator);


private:
    Backend();

private:
    static Backend* instance;
    ObjectRelational* object_relational;

};

#endif
