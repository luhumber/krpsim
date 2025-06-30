#pragma once

#include <QObject>

#include "BeamSearch.h"

class BeamSearchWorker : public QObject {
    Q_OBJECT
public:
    BeamSearchWorker(const Scenario& scenario, qint64 beam_size, qint64 max_time = 200000);

public slots:
    void process();

signals:
    void finished(QVector<BeamNode> nodes, QVector<BeamNode> solution);
    void scoreUpdated(double score);

private:
    Scenario _scenario;
    qint64 _beam_size;
    qint64 _max_time;
};
