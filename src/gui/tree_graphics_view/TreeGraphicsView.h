// TreeGraphicsView.h
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
    QHash<int, QGraphicsEllipseItem*> _nodeItems;    // id -> graphic item
    QHash<int, NodeLayout>     _layouts;              // id -> layout info
    QHash<int, QList<int>>     _childrenMap;          // parentId -> list of children ids
    double                     _currentX;             // used during recursive layout
    const double               _nodeRadius  = 20.0;   // rayon du cercle
    const double               _hSpacing    = 80.0;   // espacement horizontal minimal entre feuilles
    const double               _vSpacing    = 100.0;  // espacement vertical entre niveaux

    void computeDepths();                               // calcule la profondeur de chaque nœud
    void assignPositions();                             // calcule la position x,y de chaque nœud
    void positionRec(int nodeId);                       // fonction récursive pour le layout
    void clearData();                                   // vide les structures intermédiaires
};
