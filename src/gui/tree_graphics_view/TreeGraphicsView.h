#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QHash>
#include <QVector>
#include "BeamNode.h"

class TreeGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TreeGraphicsView(QWidget *parent = nullptr);
    void     ClearTree();

public slots:
    void on_NodesVectorCreated(const QVector<BeamNode> &nodes, 
                               const QVector<BeamNode> &current_beam);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    struct NodeLayout {
        BeamNode       node;
        int            depth;
        QPointF        pos;
    };

    QGraphicsScene             *_scene;
    QHash<int, QGraphicsEllipseItem*> _node_items;
    QHash<int, NodeLayout>     _layouts;
    QHash<int, QList<int>>     _children_map;
    double                     _current_x;
    const double               _nodeRadius  = 20.0;
    const double               _hSpacing    = 80.0;
    const double               _vSpacing    = 100.0;

    void ComputeDepths();
    void AssignPositions();
    void PositionRec(int nodeId);
    void ClearData();
};
