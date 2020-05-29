#ifndef _CONSTNAME_H
#define _CONSTNAME_H

#include <string>

namespace ConstNames
{
    const bool Overlapping = true;
    const bool Exist = true;
    const bool Permissible = true;
    const bool Equal = true;
    const std::string Empty_Str = "";
    const std::string Space_str = " ";
    const std::string New_Line = "\n";
    const std::string Standard_Room_Prefix = "s";
    const std::string Delux_Room_Prefix = "d";
    const std::string Luxury_Room_Prefix = "l";
    const std::string Premium_Room_Prefix = "p";
    const std::string User_Id_Prefix = "u";
    const int First_Room_Id = 1;
    const int First_Reserve_Id = 1;
    const int First_User_Id = 1;
    const int Precision = 2;
    const int Minimum_of_Characters = 1;
    const char Comma = ',';
    const char Amenities_Sep = '|';
    const std::string Question_Mark = "?";
    const int No_Room = 0;
    const int File_Name_Position = 1;
    const float Minimum_Deposit_Amount = 0.0f;
    const int Minimum_No_Transactions = 0;
    const char Space = ' ';
    const std::string Colon = ":";
    const std::string Hashtag = "#";
    const std::string POST_METHOD = "POST";
    const std::string GET_METHOD = "GET";
    const std::string DELETE_METHOD = "DELETE";
    const std::string Bad_Request_msg = "Bad Requset";
    const std::string Not_Found_msg = "Not Found";
    const std::string Permission_Denied_msg = "Permission Denied";
    const std::string Empty_msg = "Empty";
    const std::string OK_msg = "OK";
    const std::string Signup_Order = "signup";
    const std::string Login_Order = "login";
    const std::string Logout_Order = "logout";
    const std::string Wallet_Order = "wallet";
    const std::string Get_Hotels_Order = "hotels";
    const std::string POST_Wallet_Order = "POST wallet";
    const std::string GET_Wallet_Order = "GET wallet";
    const std::string Filter_Order = "filters";
    const std::string ID = "id";
    const std::string Email = "email";
    const std::string Amount = "amount";
    const std::string Count = "count";
    const std::string Username = "username";
    const std::string Password = "password";
    const std::string Online_User_Check = "online user check";
    const std::string User_Authentication = "user authentication";
    const std::string Room = "room";
    const std::string Star = "star";
    const std::string Overview = "overview";
    const std::string Amenities = "amenities";
    const std::string City = "city";
    const std::string Latitude = "latitude";
    const std::string Longitude = "longitude";
    const std::string Price = "price";
    const std::string Min_Star = "min_star";
    const std::string Max_Star = "max_star";
    const std::string Min_Price = "min_price";
    const std::string Max_Price = "max_price";
    const std::string StandardRoom = "standard";
    const std::string DeluxeRoom = "deluxe";
    const std::string PremiumRoom = "premium";
    const std::string LuxuryRoom = "luxury";
    const std::string Type = "type";
    const std::string Quantity = "quantity";
    const std::string Check_In = "check_in";
    const std::string Check_Out = "check_out";
    const int Minimum_Hotel_Star = 1;
    const int Maximum_Hotel_Star = 5;
    const int Minimum_Hotel_Reserve = 1;
    const int Maximum_Hotel_Reserve = 30;
}

#endif
