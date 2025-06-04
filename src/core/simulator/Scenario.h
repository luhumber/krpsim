#pragma once
#include <QStringList>
#include <QVector>
#include "Process.h"
#include "Stock.h"

struct Scenario {
    QStringList         resources;
    Stock               initial_stock; 
    QVector<Process>    processes;
    QStringList         optimize_targets;
};
