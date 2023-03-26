#include "requests.h"

#include <cmath>
#include <string>

#define FIELD(type, ...) \
    add_##type##_field(message, __VA_ARGS__)

#define add_char(character) message.push_back(character);

void add_alpha_field(std::vector<unsigned char> & message, const std::string & data, size_t max_length = 0)
{
    if (max_length == 0) {
        max_length = data.size();
    }
    for (size_t i = 0; i < max_length; ++i) {
        add_char(i < data.size() ? data[i] : ' ');
    }
}

// Добавочный метод для char, чтобы не конверировать его в string и не делать лишних операций в функции выше
void add_alpha_field(std::vector<unsigned char> & message, char data)
{
    add_char(data);
}

void add_integer_field(std::vector<unsigned char> & message, const uint32_t data)
{
    for (int byte = sizeof(data) - 1; byte >= 0; byte -= 1) {
        add_char((data >> (byte * 8)) & 0xFF);
    }
}

void add_double_field(std::vector<unsigned char> & message, const double data)
{
    FIELD(integer, data * pow(10, 4));
}

void add_time_in_force_field(std::vector<unsigned char> & message, const TimeInForce time_in_force)
{
    switch (time_in_force) {
    case TimeInForce::Day:
        add_char('0');
        break;
    case TimeInForce::IOC:
        add_char('3');
        break;
    }
}

void add_capacity_field(std::vector<unsigned char> & message, const Capacity capacity)
{
    switch (capacity) {
    case Capacity::Agency:
        add_char('1');
        break;
    case Capacity::Principal:
        add_char('2');
        break;
    case Capacity::RisklessPrincipal:
        add_char('7');
        break;
    }
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
    FIELD(alpha, 'O');

    // Order Token
    FIELD(alpha, cl_ord_id, 14);

    // Buy/Sell Indicator
    FIELD(alpha, side == Side::Buy ? 'B' : 'S');

    // Quantity
    FIELD(integer, volume);

    // Order Book
    FIELD(integer, std::stoul(symbol));

    // Price
    if (ord_type == OrdType::Limit) {
        FIELD(double, price);
    }
    else {
        FIELD(integer, 0x7FFFFFFF);
    }

    FIELD(alpha, firm, 4); // Firm
    FIELD(alpha, user, 6); // User

    FIELD(alpha, 1 | 8); // Order Bit field 1: Time in Force | Capacity
    FIELD(alpha, 0);     // Order Bit field 2
    FIELD(alpha, 0);     // Order Bit field 3
    FIELD(alpha, 0);     // Order Bit field 4

    FIELD(time_in_force, time_in_force); // Time in Force
    FIELD(capacity, capacity);           // Capacity

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
    FIELD(alpha, 'U');

    // Existing Order Token
    FIELD(alpha, old_cl_ord_id, 14);

    // Order Token
    FIELD(alpha, new_cl_ord_id, 14);

    // Quantity
    FIELD(integer, total_volume);

    // Price
    FIELD(double, price);

    // User
    FIELD(alpha, user, 6);

    FIELD(alpha, 1); // Order Bit field 1: Time in Force
    FIELD(alpha, 0); // Order Bit field 2
    FIELD(alpha, 0); // Order Bit field 3
    FIELD(alpha, 0); // Order Bit field 4

    FIELD(time_in_force, time_in_force); // Time in Force

    return message;
}
