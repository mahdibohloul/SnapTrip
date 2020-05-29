#include "CityFilter.hpp"
#include <iostream>
using namespace std;

CityFilter::CityFilter(const Database::User::FilterInfo& filter_info)
{
    city = filter_info.city;
}

CityFilter::~CityFilter() {}


void CityFilter::filter(Database::l_hotels& hotels)
{
    for(auto hotel_itr = hotels.begin(); hotel_itr != hotels.end(); hotel_itr++)
    {
        if((*hotel_itr)->get_city() == this->city)
            this->hotels.push_back(*hotel_itr);
    }
}
