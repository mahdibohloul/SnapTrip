#ifndef _AI_H
#define _AI_H

#include "../Backend.hpp"



class Backend::AI
{
public:
    friend class Backend;
    ~AI();

private:
    AI(Database::User* user);
    Database::User::Weights get_calculated_weights(Database::Hotel::v_rating ratings);
    v_double generate_random_weights();
    v_double calculate_weights(v_double& weights, Database::Hotel::v_rating ratings);
    v_double add_to_weights(v_double& weights, v_double& dirivatives);
    long double error_func(v_double weights, Database::Hotel::v_rating::iterator rating_itr);
    v_double calculate_dirivatives(v_double weights, Database::Hotel::v_rating::iterator rating_itr, v_double& dirivatives);
    long double calculate_dirivative(v_double weights, Database::Hotel::v_rating::iterator rating_itr, int which_weight);
    void clamp(v_double& numbers);
    bool check_error_funcs(v_double weights, Database::Hotel::v_rating ratings);

private:
    Database::User* user;
};

#endif
