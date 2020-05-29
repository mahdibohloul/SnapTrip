#include "AdvancedFilter.hpp"


AdvancedFilter::AdvancedFilter(const Database::User::FilterInfo& filter_info)
{
    type = filter_info.type;
    quantity = filter_info.quantity;
    check_in = filter_info.check_in;
    check_out = filter_info.check_out;
}

AdvancedFilter::~AdvancedFilter() {}

void AdvancedFilter::filter(Database::l_hotels& hotels)
{
    for(auto hotel_itr = hotels.begin(); hotel_itr != hotels.end(); hotel_itr++)
    {
        if(in_range(*hotel_itr))
            this->hotels.push_back(*hotel_itr);
    }
}

bool AdvancedFilter::in_range(const Database::Hotel *hotel)
{
    if(hotel->have_room_with_this_specifications(type, quantity, check_in, check_out))
        return true;
    return false;
}
