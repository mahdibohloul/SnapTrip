#include <iostream>
#include "./UserInterface/UserInterface.hpp"
#include "ConstNames.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
    Content file_name = argv[ConstNames::File_Name_Position];
    UserInterface* user_interface = UserInterface::get_instance();
    user_interface->database_setup(file_name);
}
