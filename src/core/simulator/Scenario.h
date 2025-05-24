#pragma once
#include "Process.h"
#include <vector>

struct Scenario {
    std::vector<std::string> resources;
    Stock   initialStock   {};
    std::vector<Process> processes;
    uint8_t optimizeResId = 0;
};