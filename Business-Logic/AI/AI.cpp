#include "AI.hpp"
#include "../../Models/Rating.hpp"
#include <iostream>
#include <math.h>
#include <algorithm>
using namespace std;


Backend::AI::AI(Database::User* user)
{
    this->user = user;
}

Backend::AI::~AI() {}

Database::User::Weights Backend::AI::get_calculated_weights(Database::Hotel::v_rating ratings)
{
    auto weights_struct = Database::User::Weights();
    auto weights = generate_random_weights();
    weights = calculate_weights(weights, ratings);
    weights_struct.initialize_weights(weights);
    return weights_struct;
}

v_double Backend::AI::generate_random_weights()
{
    v_double random_number;
    for(int i = ConstNames::Minimum_Weights_Number; i <= ConstNames::Maximum_Weights_Number; i++)
        random_number.push_back(ConstNames::Minimum_Weights_Number + static_cast<long double>(rand() / static_cast<long double>(RAND_MAX / (ConstNames::Maximum_Weights_Number - ConstNames::Minimum_Weights_Number))));
    return random_number;
}

v_double Backend::AI::calculate_weights(v_double& weights, Database::Hotel::v_rating ratings)
{
    for(int iteration = 0; iteration < ConstNames::Iterations_Number; iteration++)
    {
        auto dirivatives = v_double(ConstNames::Maximum_Weights_Number, ConstNames::Minimum_Dirivative_Amount);
        auto rating_itr = ratings.begin();
        while(rating_itr != ratings.end()){
            dirivatives = calculate_dirivatives(weights, rating_itr, dirivatives);
            rating_itr++;
        }
        weights = add_to_weights(weights, dirivatives);
        clamp(weights);
    }
    return weights;
}

v_double Backend::AI::calculate_dirivatives(v_double weights, Database::Hotel::v_rating::iterator rating_itr, v_double& dirivatives)
{
    for(int count = 0; count < ConstNames::Maximum_Weights_Number; count++)
        dirivatives[count] += calculate_dirivative(weights, rating_itr, count);
    return dirivatives;
}

long double Backend::AI::calculate_dirivative(v_double weights, Database::Hotel::v_rating::iterator rating_itr, int which_weight)
{
    long double dirivative = error_func(weights, rating_itr);
    weights[which_weight] += ConstNames::Epsilon;
    dirivative = error_func(weights, rating_itr) - dirivative;
    return dirivative / ConstNames::Epsilon;
}

long double Backend::AI::error_func(v_double weights, Database::Hotel::v_rating::iterator rating_itr)
{
    long double weighted_average = Backend::calculate_weighted_average(weights, rating_itr);
    return pow((weighted_average - (*rating_itr)->get_overall_rating()), ConstNames::Default_Exponent_Dirivative);
}

void Backend::AI::clamp(v_double& numbers)
{
    for(long double& f : numbers)
    {
        if(f < ConstNames::Minimum_Weights_Number)
            f = ConstNames::Minimum_Weights_Number;
        else if(f > ConstNames::Maximum_Weights_Number)
            f = ConstNames::Maximum_Weights_Number;
    }
}

bool Backend::AI::check_error_funcs(v_double weights, Database::Hotel::v_rating ratings)
{
    auto rating_itr = ratings.begin();
    while(rating_itr != ratings.end())
    {
        if(error_func(weights, rating_itr) > ConstNames::Epsilon)
            return false;
        rating_itr++;
    }
    return true;
}

v_double Backend::AI::add_to_weights(v_double& weights, v_double& dirivatives)
{
    for(int count = 0; count < dirivatives.size(); count++)
        weights[count] = weights[count] - (ConstNames::Step_Size * dirivatives[count]);
    return weights;
}
