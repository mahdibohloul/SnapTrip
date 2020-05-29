#include "AvgPriceFilter.hpp"

AvgPriceFilter::AvgPriceFilter(const Database::User::FilterInfo& filter_info)
{
    min_price = filter_info.min_price;
    max_price = filter_info.max_price;
}

AvgPriceFilter::~AvgPriceFilter() {}

void AvgPriceFilter::filter(Database::l_hotels& hotels)
{
    for(auto hotel_itr = hotels.begin(); hotel_itr != hotels.end(); hotel_itr++)
    {
        if(in_range(*hotel_itr))
            this->hotels.push_back(*hotel_itr);
    }
}

bool AvgPriceFilter::in_range(const Database::Hotel* hotel)
{
    if(hotel->get_avg_price() <= max_price && hotel->get_avg_price() >= min_price)
        return true;
    return false;
}
