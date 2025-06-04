#include "Parser.h"
#include "Scenario.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "RegexDefs.h"

namespace {
    void ParseResourceQuantities(const QString& groupString, Stock& resourceQuantities, QSet<QString>& allResources) {
        for (const QString& item : groupString.split(';', Qt::SkipEmptyParts)) {
            const QStringList parts = item.split(':', Qt::SkipEmptyParts);
            if (parts.size() != 2) continue;
            const QString resourceName = parts[0].trimmed();
            const int quantity = parts[1].toInt();
            resourceQuantities[resourceName] = quantity;
            allResources.insert(resourceName);
        }
    }

    bool HandleStockLine(const QString& line, Scenario& scenario, QSet<QString>& resources) {
        QRegularExpressionMatch match = Regex::stockLineRegex.match(line);
        if (!match.hasMatch()) return false;
        const QString resourceName = match.captured(1);
        const int quantity = match.captured(2).toInt();
        resources.insert(resourceName);
        scenario.initial_stock[resourceName] = quantity;
        return true;
    }

    bool HandleProcessLine(const QString& line, Scenario& scenario, QSet<QString>& resources) {
        QRegularExpressionMatch match = Regex::processLineRegex.match(line);
        if (!match.hasMatch()) return false;
        Process process;
        process.name = match.captured(1);
        ParseResourceQuantities(match.captured(2), process.needs, resources);
        if (!match.captured(3).isEmpty())
            ParseResourceQuantities(match.captured(3), process.results, resources);
        process.delay = match.captured(4).toUInt();
        scenario.processes.append(process);
        return true;
    }

    bool HandleOptimizeLine(const QString& line, Scenario& scenario) {
        QRegularExpressionMatch match = Regex::optimizeLineRegex.match(line);
        if (!match.hasMatch()) return false;
        for (const QString& target : match.captured(1).split(';', Qt::SkipEmptyParts))
            scenario.optimize_targets.append(target);
        return true;
    }
}

Scenario Parser::ParseFile(const std::filesystem::path& path) {
    Scenario scenario;
    QSet<QString> resources;
    QFile file(QString::fromStdString(path.string()));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Could not open file: " + path.string());

    QTextStream inputStream(&file);
    while (!inputStream.atEnd()) {
        QString line = inputStream.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;
        if (HandleStockLine(line, scenario, resources)) continue;
        if (HandleProcessLine(line, scenario, resources)) continue;
        if (HandleOptimizeLine(line, scenario)) continue;
        qDebug() << "Line skipped (invalid format):" << line;
    }
    scenario.resources = resources.values();
    return scenario;
}