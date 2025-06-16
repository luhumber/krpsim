#pragma once

#include <QObject>

#include "BeamSearch.h"

class BeamSearchWorker : public QObject {
    Q_OBJECT
public:
    BeamSearchWorker(const Scenario& scenario, int beam_size);

public slots:
    void process();

signals:
    void finished(QVector<BeamNode> nodes, QVector<BeamNode> solution);
    void scoreUpdated(double score);

private:
    Scenario _scenario;
    int _beam_size;
};
