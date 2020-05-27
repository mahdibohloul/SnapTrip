#include "UserInterface.hpp"
#include "../ConstNames.hpp"
#include "../Exception/Exception.hpp"
#include <iostream>
using namespace std;


UserInterface* UserInterface::instance = nullptr;

UserInterface* UserInterface::get_instance()
{
    if(instance == nullptr)
        instance = new UserInterface();
    return instance;
}

void UserInterface::release()
{
    delete instance;
    instance = nullptr;
}

UserInterface::UserInterface()
{
    api = API::get_instance();
}

UserInterface::~UserInterface() {}


void UserInterface::database_setup(Content file_name)
{
    ifstream db_file(file_name);
    Content content;
    dump_one_line(db_file);
    while(db_file)
    {
        getline(db_file, content);
        if(content.size() >= ConstNames::Minimum_of_Characters)
        {
            Backend::data_t data = api->parse_content(content, ConstNames::Comma);
            api->database_setup(data);
        }
        else
            break;
    }
}

void UserInterface::dump_one_line(ifstream& file)
{
    Content dump;
    getline(file, dump);
}

void UserInterface::process()
{
    string command;
    while(getline(cin, command))
    {
        try
        {
            Backend::data_t command_data = api->parse_content(command, ConstNames::Space);
            cout << api->command_processor(command_data) << endl;
        }
        catch(Exception ex)
        {
            cerr << ex.what() << endl;
        }
    }
}

