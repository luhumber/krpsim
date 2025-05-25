#include "Parser.h"
#include "Scenario.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpressionMatch>
#include "RegexDefs.h"

namespace {
    void parseResourceQuantities(const QString& groupString, Stock& resourceQuantities, QStringList& allResources) {
        const QStringList items = groupString.split(';', Qt::SkipEmptyParts);
        for (const QString& item : items) {
            const QStringList parts = item.split(':', Qt::SkipEmptyParts);
            if (parts.size() != 2)
                continue;
            const QString resourceName = parts[0].trimmed();
            const int quantity = parts[1].toInt();
            resourceQuantities[resourceName] = quantity;
            if (!allResources.contains(resourceName))
                allResources.append(resourceName);
        }
    }
}

Scenario Parser::parseFile(const std::filesystem::path& path) {
    Scenario scenario;
    QFile file(QString::fromStdString(path.string()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Could not open file: " + path.string());

    QTextStream inputStream(&file);
    while (!inputStream.atEnd()) {
        QString line = inputStream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#'))
            continue;
        
        qDebug() << "=====================================";

        if (Parser::handleStockLine   (line, scenario))   continue;
        if (Parser::handleProcessLine (line, scenario))   continue;
        if (Parser::handleOptimizeLine(line, scenario))   continue;

        qDebug() << "Line skipped (invalid format):" << line;
    }

    return scenario;
}

bool Parser::handleOptimizeLine(const QString& line, Scenario& scenario) {
    QRegularExpressionMatch match = Regex::optimizeLineRegex.match(line);
    if (!match.hasMatch()) return false;

    for (const QString& target : match.captured(1).split(';', Qt::SkipEmptyParts)) {
        scenario.optimizeTargets.append(target);
        qDebug() << "Optimize target:" << target;
    }
    return true;
}

bool Parser::handleStockLine(const QString& line, Scenario& scenario) {
    QRegularExpressionMatch match = Regex::stockLineRegex.match(line);
    if (!match.hasMatch()) return false;

    const QString resourceName = match.captured(1);
    const int quantity = match.captured(2).toInt();
    scenario.resources.append(resourceName);
    scenario.initialStock[resourceName] = quantity;
    qDebug() << "Initial stock parsed:" << resourceName << ":" << quantity;
    return true;
}

bool Parser::handleProcessLine(const QString& line, Scenario& scenario) {
    QRegularExpressionMatch match = Regex::processLineRegex.match(line);
    if (!match.hasMatch()) return false;
    qDebug() << "Processing line for process:" << line;

    Process process;
    process.name = match.captured(1);

    parseResourceQuantities(match.captured(2), process.needs, scenario.resources);
    if (!match.captured(3).isEmpty())
        parseResourceQuantities(match.captured(3), process.results, scenario.resources);

    qDebug() << "Process parsed:" << process.name;
    qDebug() << "Needs:";
    for (const auto& need : process.needs.toStdMap()) {
        qDebug() << "  " << need.first << ":" << need.second;
    }
    qDebug() << "Results:";
    for (const auto& result : process.results.toStdMap()) {
        qDebug() << "  " << result.first << ":" << result.second;
    }

    process.delay = match.captured(4).toUInt();
    qDebug() << "Delay parsed for process:" << process.name << ":" << process.delay;

    scenario.processes.append(process);
    qDebug() << "Process appended:" << process.name;
    return true;
}