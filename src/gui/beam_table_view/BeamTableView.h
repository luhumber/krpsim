#pragma once

#include <QTableView>
#include "BeamNode.h"

class BeamTableView : public QTableView
{
    Q_OBJECT
public:
    explicit    BeamTableView(QWidget *parent = nullptr);
    void        DisplaySolutionSteps(const QVector<BeamNode>& solution);
    void        ClearTable();
};
