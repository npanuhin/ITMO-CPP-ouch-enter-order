#include "requests.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

void add_alpha_field(std::vector<unsigned char> & message, const std::string & data, size_t length)
{
    for (size_t i = 0; i < length; ++i) {
        message.push_back(i < data.size() ? data[i] : ' ');
    }
}

void add_integer_field(std::vector<unsigned char> & message, uint32_t data)
{
    for (int i = 24; i >= 0; i -= 8) {
        message.push_back(static_cast<unsigned char>((data >> i) & 0xFF));
    }
}

void add_double_field(std::vector<unsigned char> & message, double data)
{
    std::cout << std::dec << "double_data:" << data << std::endl;
    unsigned long long integer_data = (data * pow(10, 4));
    std::cout << std::dec << "integer_data:" << integer_data << std::endl;
    integer_data %= static_cast<unsigned long long>(pow(10, 10));
    std::cout << std::dec << "integer_data:" << integer_data << std::endl;
    add_integer_field(message, integer_data);
}

// void add_bitmask_field(std::vector<unsigned char> & message, char bitmask) {
//     std::cout << "bitmask: " << bitmask << std::endl;
//     // for (size_t i = 0; i < length; ++i) {
//     //     message.push_back('0' + (bitmask & 1));
//     //     bitmask >>= 1;
//     // }
//     message.push_back(bitmask);
// }

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

    // Firm
    add_alpha_field(message, firm, 4);

    // User
    add_alpha_field(message, user, 6);

    // Order Bit field 1
    message.push_back(1 | 8); // Bitmask: Time in Force | Capacity

    // Order Bit field 2
    message.push_back(0); // Bitmask

    // Order Bit field 3
    message.push_back(0); // Bitmask

    // Order Bit field 4
    message.push_back(0); // Bitmask

    // Time in Force
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

    // Capacity
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

    std::cout << message.size() << std::endl;

    for (const auto ch : message) {
        std::cout << std::hex << static_cast<int>(ch) << ' ';
    }
    std::cout << std::endl;

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

    // Order Bit field 1
    message.push_back(1); // Bitmask: Time in Force

    // Order Bit field 2
    message.push_back(0); // Bitmask

    // Order Bit field 3
    message.push_back(0); // Bitmask

    // Order Bit field 4
    message.push_back(0); // Bitmask

    // Time in Force
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

    std::cout << message.size() << std::endl;

    for (const auto ch : message) {
        std::cout << std::hex << static_cast<int>(ch) << ' ';
    }
    std::cout << std::endl;

    return message;
}
