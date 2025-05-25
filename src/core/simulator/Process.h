#pragma once
#include "Stock.h"
#include <QString>

struct Process {
    QString name;
    Stock   needs   {};
    Stock   results {};
    uint16_t delay = 0;
};