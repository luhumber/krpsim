#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <iostream>

#include "Parser.h"
#include "Stock.h"
#include "Scenario.h"

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    if (argc < 3) {
        std::cerr << COLOR_YELLOW << "Usage: krpsim_verif <scenario.krpsim> <trace.txt>" << COLOR_RESET << "\n";
        return 1;
    }

    QString scenarioPath = argv[1];
    QString traceFilePath = argv[2];

    QFile scenarioFile(scenarioPath);
    if (!scenarioFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << COLOR_RED << "Error: Unable to open scenario file." << COLOR_RESET << "\n";
        return 1;
    }

    QTextStream scenarioStream(&scenarioFile);
    Scenario scenario = Parser::ParseFile(std::filesystem::path(scenarioPath.toStdString()));

    QFile traceFile(traceFilePath);
    if (!traceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << COLOR_RED << "Error: Unable to open trace file." << COLOR_RESET << "\n";
        return 1;
    }

    QTextStream in(&traceFile);
    Stock currentStock = scenario.initial_stock;
    int lastCycle = 0;
    bool ok = true;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;

        QStringList parts = line.split(':', Qt::SkipEmptyParts);
        if (parts.size() != 2) {
            std::cout << COLOR_RED << "Invalid trace format at line: " << line.toStdString() << COLOR_RESET << "\n";
            ok = false;
            break;
        }

        int cycle = parts[0].toInt();
        QString processName = parts[1];

        if (cycle < lastCycle) {
            std::cout << COLOR_RED << "Inconsistent cycle order at line: " << line.toStdString() << COLOR_RESET << "\n";
            ok = false;
            break;
        }
        lastCycle = cycle;

        const Process* proc = nullptr;
        for (const Process& p : scenario.processes) {
            if (p.name == processName) {
                proc = &p;
                break;
            }
        }

        if (!proc) {
            std::cout << COLOR_RED << "Unknown process at line: " << line.toStdString() << COLOR_RESET << "\n";
            ok = false;
            break;
        }

        if (!currentStock.CanApplyProcess(proc->needs)) {
            std::cout << COLOR_RED << "Insufficient stock for process at line: " << line.toStdString() << COLOR_RESET << "\n";
            ok = false;
            break;
        }

        currentStock.ApplyProcess(proc->needs, proc->results);
    }

    bool goalReached = true;
    for (const QString& target : scenario.optimize_targets) {
        if (target != "time" && currentStock.value(target) <= 0) {
            std::cout << COLOR_YELLOW << "Optimization target not reached for resource: " << target.toStdString() << COLOR_RESET << "\n";
            goalReached = false;
        }
    }

    if (ok && goalReached) {
        std::cout << COLOR_GREEN << "The trace is valid." << COLOR_RESET << "\n";
    } else {
        std::cout << COLOR_RED << "The trace is invalid." << COLOR_RESET << "\n";
    }

    std::cout << "Final stock: " << currentStock.toString().toStdString() << "\n";
    std::cout << "Last cycle: " << lastCycle << "\n";

    return ok && goalReached ? 0 : 1;
}
