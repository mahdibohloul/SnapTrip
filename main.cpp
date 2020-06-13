#include <iostream>
#include "./UserInterface/UserInterface.hpp"
#include "ConstNames.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
    Content file_name = argv[ConstNames::Hotel_File_Addr_Position];
    Content rating_file = argv[ConstNames::Rating_File_Addr_Position];
    UserInterface* user_interface = UserInterface::get_instance();
    user_interface->database_setup(file_name);
    user_interface->rating_db_setup(rating_file);
    user_interface->process();
    UserInterface::release();
}
