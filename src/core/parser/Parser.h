#pragma once
#include "Scenario.h"
#include <filesystem>

class Parser {
public:
    static Scenario ParseFile(const std::filesystem::path& path);
};
