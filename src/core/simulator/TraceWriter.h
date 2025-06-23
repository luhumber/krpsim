#pragma once

#include <QVector>
#include "BeamNode.h"

class TraceWriter {
public:
    void writeTrace(const QVector<BeamNode>& solutionPath);
};