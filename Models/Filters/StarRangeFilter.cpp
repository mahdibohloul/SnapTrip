#include "StarRangeFilter.hpp"



StarRangeFilter::StarRangeFilter(const Database::User::FilterInfo& filter_info)
{
    min_star = filter_info.min_star;
    max_star = filter_info.max_star;
}

StarRangeFilter::~StarRangeFilter() {}

void StarRangeFilter::filter(Database::l_hotels& hotels)
{
    if(activity != ConstNames::Active_Mode)
        this->hotels = hotels;
    else
    {
        for(auto hotel_itr = hotels.begin(); hotel_itr != hotels.end(); hotel_itr++)
        {
            if(in_range(*hotel_itr))
                this->hotels.push_back(*hotel_itr);
        }
    }
}

bool StarRangeFilter::in_range(const Database::Hotel* hotel)
{
    if(hotel->get_star() <= max_star && hotel->get_star() >= min_star)
        return true;
    return false;
}
