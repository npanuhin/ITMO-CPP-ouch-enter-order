#include "requests.h"

#include <string>

std::vector<unsigned char> create_enter_order_request(
        const std::string & /*cl_ord_id*/,
        const Side /*side*/,
        const double /*volume*/,
        const double /*price*/,
        const std::string & /*symbol*/,
        const OrdType /*ord_type*/,
        const TimeInForce /*time_in_force*/,
        const Capacity /*capacity*/,
        const std::string & /*firm*/,
        const std::string & /*user*/
)
{
    /* Write your code here */;
    return {};
}

std::vector<unsigned char> create_replace_order_request(
        const std::string & /*old_cl_ord_id*/,
        const std::string & /*new_cl_ord_id*/,
        const double /*total_volume*/,
        const double /*price*/,
        const TimeInForce /*time_in_force*/,
        const std::string & /*user*/
)
{
    /* Write your code here */;
    return {};
}
