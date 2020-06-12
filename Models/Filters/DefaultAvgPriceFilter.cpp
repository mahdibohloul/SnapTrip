#include "DefaultAvgPriceFilter.hpp"
#include "../User/ReserveCase.hpp"
#include <math.h>
using namespace std;

DefaultAvgPriceFilter::DefaultAvgPriceFilter(const Database::User::l_booked& booked_rooms)
{
    average = calculate_average(booked_rooms);
    average_deviation = calculate_average_deviation(booked_rooms);
}

DefaultAvgPriceFilter::~DefaultAvgPriceFilter() {}


float DefaultAvgPriceFilter::calculate_average(const Database::User::l_booked& booked_rooms)
{
    float cost = 0.0f;
    int quantity = 0;
    for(auto reserve : booked_rooms)
    {
        cost += reserve->get_cost_of_reserves() / reserve->get_residency_period();
        quantity += reserve->get_quantity_of_reserves();
    }
    if(quantity == 0)
        return 0.0f;
    return cost / quantity;
}

float DefaultAvgPriceFilter::calculate_average_deviation(const Database::User::l_booked& booked_rooms)
{
    float deviation = 0.0f, sum = 0.0f;
    int quantity = 0;
    for(auto reserve : booked_rooms)
    {
        sum += (pow(((reserve->get_cost_of_reserves() / reserve->get_residency_period()) / reserve->get_quantity_of_reserves()) - average, 2))* reserve->get_quantity_of_reserves();
        quantity += reserve->get_quantity_of_reserves();
    }
    if(quantity == 1)
        return 0.0f;
    deviation = sum / (quantity - 1);
    return sqrt(deviation);
}

void DefaultAvgPriceFilter::filter(Database::l_hotels& hotels)
{
    for(auto hotel : hotels)
    {
        if(in_range(hotel->get_avg_price()))
            this->hotels.push_back(hotel);
    }
}

bool DefaultAvgPriceFilter::in_range(float hotel_avg_price)
{
    float abs_val = abs(hotel_avg_price - average);
    if(abs_val <= (ConstNames::Default_Filter_Coefficient * average_deviation))
        return true;
    return false;
}
