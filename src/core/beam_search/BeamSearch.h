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
    explicit BeamSearch(const Scenario& scenario, int beam_size, double max_time = 200000.0);
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
    double _time_penalty = 1;
    double _max_time;
    QHash<QString, int> _seenStockBestTime;
};
