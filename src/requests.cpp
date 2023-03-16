#include "requests.h"

#include <cmath>
#include <string>

void add_alpha_field(std::vector<unsigned char> & message, const std::string & data, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        message.push_back(i < data.size() ? data[i] : ' ');
    }
}

void add_integer_field(std::vector<unsigned char> & message, const uint32_t data)
{
    for (int bits = 24; bits >= 0; bits -= 8) {
        message.push_back(static_cast<unsigned char>((data >> bits) & 0xFF));
    }
}

void add_double_field(std::vector<unsigned char> & message, const double data)
{
    add_integer_field(message, data * pow(10, 4));
}

void add_time_in_force_field(std::vector<unsigned char> & message, const TimeInForce time_in_force)
{
    std::string time_in_force_value;
    switch (time_in_force) {
    case TimeInForce::Day:
        time_in_force_value = "0";
        break;
    case TimeInForce::IOC:
        time_in_force_value = "3";
        break;
    }
    add_alpha_field(message, time_in_force_value, 1);
}

void add_capacity_field(std::vector<unsigned char> & message, const Capacity capacity)
{
    std::string capacity_value;
    switch (capacity) {
    case Capacity::Agency:
        capacity_value = "1";
        break;
    case Capacity::Principal:
        capacity_value = "2";
        break;
    case Capacity::RisklessPrincipal:
        capacity_value = "7";
        break;
    }
    add_alpha_field(message, capacity_value, 1);
}

std::vector<unsigned char> create_enter_order_request(
        const std::string & cl_ord_id,
        const Side side,
        const double volume,
        const double price,
        const std::string & symbol,
        const OrdType ord_type,
        const TimeInForce time_in_force,
        const Capacity capacity,
        const std::string & firm,
        const std::string & user)
{
    std::vector<unsigned char> message;

    // Type
    message.push_back('O');

    // Order Token
    add_alpha_field(message, cl_ord_id, 14);

    // Buy/Sell Indicator
    message.push_back(side == Side::Buy ? 'B' : 'S');

    // Quantity
    add_integer_field(message, volume);

    // Order Book
    add_integer_field(message, std::stoull(symbol)); // stoul can be used

    // Price
    if (ord_type == OrdType::Limit) {
        add_double_field(message, price);
    }
    else {
        add_integer_field(message, 0x7FFFFFFF);
    }

    add_alpha_field(message, firm, 4); // Firm
    add_alpha_field(message, user, 6); // User

    message.push_back(1 | 8); // Order Bit field 1: Time in Force | Capacity
    message.push_back(0);     // Order Bit field 2
    message.push_back(0);     // Order Bit field 3
    message.push_back(0);     // Order Bit field 4

    add_time_in_force_field(message, time_in_force); // Time in Force
    add_capacity_field(message, capacity);           // Capacity

    return message;
}

std::vector<unsigned char> create_replace_order_request(
        const std::string & old_cl_ord_id,
        const std::string & new_cl_ord_id,
        const double total_volume,
        const double price,
        const TimeInForce time_in_force,
        const std::string & user)
{
    std::vector<unsigned char> message;

    // Type
    message.push_back('U');

    // Existing Order Token
    add_alpha_field(message, old_cl_ord_id, 14);

    // Order Token
    add_alpha_field(message, new_cl_ord_id, 14);

    // Quantity
    add_integer_field(message, total_volume);

    // Price
    add_double_field(message, price);

    // User
    add_alpha_field(message, user, 6);

    message.push_back(1); // Order Bit field 1: Time in Force
    message.push_back(0); // Order Bit field 2
    message.push_back(0); // Order Bit field 3
    message.push_back(0); // Order Bit field 4

    add_time_in_force_field(message, time_in_force); // Time in Force

    return message;
}
