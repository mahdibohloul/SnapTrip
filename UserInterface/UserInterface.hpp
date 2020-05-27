#ifndef _USERINTERFACE_H
#define _USERINTERFACE_H

#include <string>
#include <fstream>
#include <vector>
#include "../API/API.hpp"


typedef std::string Content;

class UserInterface
{
public:
    ~UserInterface();
    static UserInterface* get_instance();
    static void release();
    void database_setup(Content file_name);
    void process();

private:
    UserInterface();
    void dump_one_line(std::ifstream& file);

private:
    static UserInterface* instance;
    API* api;


};







#endif
