#include "TraceWriter.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "BeamNode.h"

void TraceWriter::WriteTrace(const QVector<BeamNode>& solution_path) {
    qDebug() << "Writing trace to file...";
    if (solution_path.isEmpty()) {
        qDebug() << "No solution path to write.";
        return;
    }
    
    QDir current_dir = QDir::current();
    
    if (!current_dir.cdUp()) {
        qDebug() << "Failed to access parent directory";
        return;
    }
    
    if (!current_dir.exists("output")) {
        if (!current_dir.mkdir("output")) {
            qDebug() << "Failed to create output directory";
            return;
        }
    }
    
    QString file_path = current_dir.filePath("output/tracefile.txt");
    QFile trace_file(file_path);
    
    if (!trace_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << trace_file.errorString();
        return;
    }
    
    QTextStream out(&trace_file);
    for (const BeamNode& node : solution_path) {
        if (node.getProcessName() == "START")
            continue;
        out << node.getState().time << ":" << node.getProcessName() << "\n";
    }
    
    trace_file.close();
    qDebug() << "Trace written to" << file_path;
}