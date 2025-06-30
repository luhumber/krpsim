#pragma once

#include <QVector>
#include <QHash>
#include <QString>
#include <QDebug>
#include <QElapsedTimer>
#include <QObject>

#include "Scenario.h"
#include "BeamNode.h"
#include "BeamState.h"

class BeamSearch {
public:
    explicit BeamSearch(const Scenario& scenario, qint64 beam_size, qint64 max_time);
    void RunAlgorithm();
    QVector<BeamNode> getSolutionPath() const;
    QVector<BeamNode> getNodesVector() const { return _nodes_vector; }
    void setScoreCallback(std::function<void(double)> cb) { _scoreCallback = std::move(cb); }
    const QVector<double>& getMaxScorePerIteration() const { return _maxScorePerIteration; }

private:
    double ComputeScore(const BeamState& state, const Scenario& scenario) const;
    void ExpandBeam(QVector<BeamNode>& current_beam, int& nextId);
    const BeamNode* getBestNode() const;
    int getSumTargets(const BeamState& state) const;

    const Scenario&     _scenario;
    qint64                 _beam_size;
    QVector<BeamNode>   _current_beam;
    QVector<BeamNode>   _nodes_vector;
    double              _time_penalty = 1;
    qint64              _max_time;
    QHash<QString, int> _seenStockBestTime;
    std::function<void(double)> _scoreCallback;
    QVector<double> _maxScorePerIteration;
};
