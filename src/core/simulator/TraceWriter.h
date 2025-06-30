#pragma once

#include <QVector>
#include "BeamNode.h"

class TraceWriter {
public:
    void WriteTrace(const QVector<BeamNode>& solution_path);
};