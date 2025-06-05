#pragma once

#include <QVector>
#include <QHash>
#include <QString>
#include <QDebug>
#include <QElapsedTimer>

#include "Scenario.h"
#include "BeamNode.h"
#include "BeamState.h"

class BeamSearch {
public:
    BeamSearch(const Scenario& scenario, int beam_size);
    void RunAlgorithm();
    QVector<BeamNode> getSolutionPath() const;
    QVector<BeamNode> getNodesVector() const { return _nodes_vector; }

private:
    double ComputeScore(const BeamState& state, const Scenario& scenario) const;
    void ExpandBeam(QVector<BeamNode>& current_beam, int& nextId);
    const BeamNode* getBestNode() const;
    int getSumTargets(const BeamState& state) const;

    const Scenario& _scenario;
    int _beam_size;
    QVector<BeamNode> _current_beam;
    QVector<BeamNode> _nodes_vector;
    double _time_weight = 0.5;
};
