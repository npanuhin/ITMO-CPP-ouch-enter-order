#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <vector>

enum class RequestType
{
    EnterOrder
};

enum class Side
{
    Buy,
    Sell
};

enum class OrdType
{
    Market,
    Limit
};

enum class TimeInForce
{
    Day,
    IOC
};

enum class Capacity
{
    Agency,
    Principal,
    RisklessPrincipal
};

std::vector<unsigned char> create_enter_order_request(
        const std::string & cl_ord_id,
        Side side,
        double volume,
        double price,
        const std::string & symbol,
        OrdType ord_type,
        TimeInForce time_in_force,
        Capacity capacity,
        const std::string & firm,
        const std::string & user);

std::vector<unsigned char> create_replace_order_request(
        const std::string & old_cl_ord_id,
        const std::string & new_cl_ord_id,
        double total_volume,
        double price,
        TimeInForce time_in_force,
        const std::string & user);
