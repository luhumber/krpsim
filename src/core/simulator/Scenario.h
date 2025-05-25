#pragma once
#include <QStringList>
#include <QVector>
#include "Process.h"
#include "Stock.h"

struct Scenario {
    QStringList         resources;
    Stock               initialStock; 
    QVector<Process>    processes;
    QStringList         optimizeTargets;
};