#ifndef _OBJECTRELATIONAL_H
#define _OBJECTRELATIONAL_H

#include "../Models/Database.hpp"
#include "../Business-Logic/Backend.hpp"

enum Data_Enum
{
    e_id,
    e_name,
    e_star_rating,
    e_overview,
    e_property_amenities,
    e_city,
    e_latitude,
    e_longitude,
    e_img_url,
    e_no_s_rooms,
    e_no_d_rooms,
    e_no_l_rooms,
    e_no_p_rooms,
    e_s_price,
    e_d_price,
    e_l_price,
    e_p_price
};

class ObjectRelational
{
public:
    static ObjectRelational* get_instance();
    static void release();
    ~ObjectRelational();

    void database_setup(Backend::data_t data);
    void deposit_in(const Database::UserInfo& user_info, Database::User* user);
    void set_filter(const Database::User::FilterInfo& filter_info, Database::User* user);
    void delete_filter(Database::User* user);
    void post_comment(Database::User* user, std::string hotel_id, std::string comment);
    void logout_user(Database::User* user);
    void post_rating(Database::Hotel::RatingInfo& rating_info);

    bool authentication(Database::UserInfo user_info, const std::string mode, const Database::l_users& users = Database::l_users());

    Content print_account_information(const Database::UserInfo& user_info, Database::User* user);
    Content get_hotels(Database::User* user);
    Content get_hotels(info_t hotel_id);
    Content booked_out(Database::User::ReserveInfo& reserve_info, Database::User* user);
    Content get_reserve(Database::User* user);
    Content get_comment(std::string hotel_id);
    Content get_rating(std::string hotel_id);
    Content delete_reserve(Database::User* user, int id);

    Database::User* get_specific_user(Database::UserInfo user_info, const std::string& mode);

private:
    ObjectRelational();

private:
    Database* database;
    Backend* backend;
    static ObjectRelational* instance;

};


#endif
