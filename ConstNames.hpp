#ifndef _CONSTNAME_H
#define _CONSTNAME_H

#include <string>

namespace ConstNames
{
    const bool Not_Booked = false;
    const bool Booked = true;
    const std::string Standard_Room_Prefix = "s";
    const std::string Delux_Room_Prefix = "d";
    const std::string Luxury_Room_Prefix = "l";
    const std::string Premium_Room_Prefix = "p";
    const int First_Room_Id = 1;
    const int First_Reserve_Id = 1;
    const int Minimum_of_Characters = 1;
    const char Comma = ',';
    const char Amenities_Sep = '|';
    const int No_Room = 0;
    const int File_Name_Position = 1;
}

#endif
