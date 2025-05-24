#pragma once
#include "Stock.hpp"
#include <string_view>

struct Process {
    std::string_view name;
    Stock   needs   {};
    Stock   results {};
    uint16_t delay = 0;
};