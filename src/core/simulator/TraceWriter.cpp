#include "TraceWriter.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "BeamNode.h"

void TraceWriter::writeTrace(const QVector<BeamNode>& solutionPath) {
    qDebug() << "Writing trace to file...";
    if (solutionPath.isEmpty()) {
        qDebug() << "No solution path to write.";
        return;
    }
    
    QDir currentDir = QDir::current();
    
    if (!currentDir.cdUp()) {
        qDebug() << "Failed to access parent directory";
        return;
    }
    
    if (!currentDir.exists("output")) {
        if (!currentDir.mkdir("output")) {
            qDebug() << "Failed to create output directory";
            return;
        }
    }
    
    QString filePath = currentDir.filePath("output/tracefile.txt");
    QFile traceFile(filePath);
    
    if (!traceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << traceFile.errorString();
        return;
    }
    
    QTextStream out(&traceFile);
    for (const BeamNode& node : solutionPath) {
        if (node.getProcessName() == "START")
            continue;
        out << node.getState().time << ":" << node.getProcessName() << "\n";
    }
    
    traceFile.close();
    qDebug() << "Trace written to" << filePath;
}