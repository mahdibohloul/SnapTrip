#ifndef _CONSTNAME_H
#define _CONSTNAME_H

#include <string>

namespace ConstNames
{
    const bool Overlapping = true;
    const bool Exist = true;
    const bool Permissible = true;
    const bool Equal = true;
    const bool Active_Mode = true;
    const bool Inactive_Mode = false;

    const int First_Room_Id = 1;
    const int First_Reserve_Id = 1;
    const int First_User_Id = 1;
    const int Precision = 2;
    const int Minimum_of_Characters = 1;
    const int No_Room = 0;
    const int One_Night = 1;
    const int Minimum_Ratings_Number = 5;
    const int Hotel_File_Addr_Position = 1;
    const int Rating_File_Addr_Position = 2;
    const int Minimum_No_Transactions = 0;
    const int Minimum_Hotel_Star = 1;
    const int Maximum_Hotel_Star = 5;
    const int Maximum_Weights_Number = 5;
    const int Minimum_Weights_Number = 1;
    const int Minimum_Hotel_Reserve = 1;
    const int Maximum_Hotel_Reserve = 30;
    const int Iterations_Number = 1000;
    const int Default_Filter_Coefficient = 2;
    const int Default_Exponent_Dirivative = 2;
    const int Default_Star = 0;
    const int Default_Quantity = -1;
    const int Default_Price = -1;
    const int Default_Reservation = 0;
    const int Minimum_Reservation_Count = 10;
    const int Reverse = -1;
    const int Smaller_cmp = -1;
    const int Equal_cmp = 0;
    const int Greater_cmp = 1;
    const long double Minimum_Deposit_Amount = 0.0f;
    const long double Min_Rate = 1.0;
    const long double Max_Rate = 5.0;
    const long double Fifty_Percent = 0.5;
    const long double Minimum_Dirivative_Amount = 0.00;
    const long double Step_Size = 5.0;
    const long double Epsilon = 0.0001;

    const std::string Empty_Str = "";
    const std::string Space_str = " ";
    const std::string New_Line = "\n";
    const char Comma = ',';
    const char Amenities_Sep = '|';
    const std::string Question_Mark = "?";
    const char Space = ' ';
    const std::string Colon = ":";
    const std::string Hashtag = "#";
    const std::string Standard_Room_Prefix = "s";
    const std::string Delux_Room_Prefix = "d";
    const std::string Luxury_Room_Prefix = "l";
    const std::string Premium_Room_Prefix = "p";
    const std::string User_Id_Prefix = "u";
    const std::string Dump_str_Number = "1";

    const std::string POST_METHOD = "POST";
    const std::string GET_METHOD = "GET";
    const std::string DELETE_METHOD = "DELETE";

    const std::string Bad_Request_msg = "Bad Requset";
    const std::string Not_Found_msg = "Not Found";
    const std::string Permission_Denied_msg = "Permission Denied";
    const std::string Not_Enough_Room_msg = "Not Enough Room";
    const std::string Not_Enough_Credit_msg = "Not Enough Credit";
    const std::string No_Rating_msg = "No Rating";
    const std::string Empty_msg = "Empty";
    const std::string OK_msg = "OK";
    const std::string Insufficient_Ratings_msg = "Insufficient Ratings";
    const std::string Default_Filter_msg = "* Results have been filtered by the default price filter.";

    const std::string Signup_Order = "signup";
    const std::string Login_Order = "login";
    const std::string Logout_Order = "logout";
    const std::string Wallet_Order = "wallet";
    const std::string Get_Hotels_Order = "hotels";
    const std::string Filter_Order = "filters";
    const std::string Default_Price_Filter_Order = "default_price_filter";
    const std::string Reserve_Order = "reserves";
    const std::string Comment_Order = "comments";
    const std::string Rating_Order = "ratings";
    const std::string POST_Wallet_Order = "POST Wallet";
    const std::string GET_Wallet_Order = "GET Wallet";
    const std::string Sort_Order = "sort";
    const std::string Manual_Weights_Order = "manual_weights";
    const std::string Estimated_Weights_Order = "estimated_weights";

    const std::string ID = "id";
    const std::string Email = "email";
    const std::string Amount = "amount";
    const std::string Count = "count";
    const std::string Username = "username";
    const std::string Password = "password";
    const std::string Online_User_Check = "online user check";
    const std::string User_Authentication = "user authentication";
    const std::string Room = "room";
    const std::string Rooms = "rooms";
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
    const std::string Cost = "cost";
    const std::string Hotel = "hotel";
    const std::string Comment = "comment";
    const std::string Location = "location";
    const std::string Cleanliness = "cleanliness";
    const std::string Facilities = "facilities";
    const std::string Value_For_Money = "value_for_money";
    const std::string Overall_Rating = "overall_rating";
    const std::string Staff = "staff";
    const std::string Value_For_Money_sep = "value for money";
    const std::string Overall_Rating_sep = "overall rating";
    const std::string Active = "active";
    const std::string True = "true";
    const std::string False = "false";
    const std::string Online_Mode = "online mode";
    const std::string Offline_Mode = "offline mode";
    const std::string Property = "property";
    const std::string Order = "order";
    const std::string Name = "name";
    const std::string Star_Rating = "star_rating";
    const std::string Standard_Room_Price = "standard_room_price";
    const std::string Deluxe_Room_Price = "deluxe_room_price";
    const std::string Luxury_Room_Price = "luxury_room_price";
    const std::string Premium_Room_Price = "premium_room_price";
    const std::string Average_Room_Price = "average_room_price";
    const std::string Rating_Overall = "rating_overall";
    const std::string Rating_Personal = "rating_personal";
    const std::string Ascending = "ascending";
    const std::string Descending = "descending";
}

#endif
