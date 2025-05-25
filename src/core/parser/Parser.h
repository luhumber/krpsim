#pragma once
#include "Scenario.h"
#include <filesystem>

#include <QFileDialog>
#include <QFileInfo>
#include <QDir>

class Parser {

public:
    Scenario parseFile(const std::filesystem::path& path);

private:
    bool handleOptimizeLine(const QString& line, Scenario& s);
    bool handleStockLine   (const QString& line, Scenario& s);
    bool handleProcessLine (const QString& line, Scenario& s);
};