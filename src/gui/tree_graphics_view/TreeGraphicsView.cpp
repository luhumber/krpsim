// TreeGraphicsView.cpp
#include "TreeGraphicsView.h"
#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QQueue>

TreeGraphicsView::TreeGraphicsView(QWidget *parent)
    : QGraphicsView(parent),
      _scene(new QGraphicsScene(this)),
      _currentX(0.0)
{
    setScene(_scene);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void TreeGraphicsView::clearData()
{
    _layouts.clear();
    _childrenMap.clear();
    _nodeItems.clear();
    _currentX = 0.0;
}

void TreeGraphicsView::on_NodesVectorCreated(const QVector<BeamNode> &nodes,
                                              const QVector<BeamNode> &current_beam)
{
    qDebug() << "Received Current Beam with" << current_beam.size() << "nodes.";
    // 1. Nettoyer la scène et les données précédentes
    _scene->clear();
    clearData();

    // 2. Construire le map id -> BeamNode et parent -> liste d'enfants
    for (const BeamNode &bn : nodes) {
        int id       = bn.getId();
        int parentId = bn.getParentId();
        _layouts[id] = { bn, -1, QPointF() };  // depth à calculer
        // Inscrire cet id comme enfant de parentId (même si parentId == -1)
        _childrenMap[parentId].append(id);
    }

    // 3. Calculer la profondeur (depth) de chaque nœud
    computeDepths();

    // 4. Calculer les positions (x, y) via un parcours récursif
    assignPositions();

    // 5. Créer les items graphiques (cercles) aux positions calculées
    QPen   nodePen(Qt::black);
    QBrush normalBrush(Qt::yellow);
    QBrush solutionBrush(Qt::green);

    QSet<int> beamIds;
    for (const BeamNode &bn : current_beam) {
        beamIds.insert(bn.getId());
    }

    for (auto it = _layouts.constBegin(); it != _layouts.constEnd(); ++it) {
        int id = it.key();
        const NodeLayout &nl = it.value();

        const QBrush &brush = beamIds.contains(id) ? solutionBrush : normalBrush;

        // Les coordonnées fournies dans nl.pos représentent le centre du nœud
        double x = nl.pos.x() - _nodeRadius;
        double y = nl.pos.y() - _nodeRadius;

        QGraphicsEllipseItem *ellipse = _scene->addEllipse(
            x, y,
            _nodeRadius * 2.0, _nodeRadius * 2.0,
            nodePen, brush
        );
        ellipse->setZValue(1);
        ellipse->setToolTip(QString("Node %1: %2, Score: %3").arg(id).arg(nl.node.getProcessName()).
                            arg(nl.node.score()));
        _nodeItems[id] = ellipse;
    }

    // 6. Tracer les arêtes (lignes) parent -> enfant
    QPen edgePen(Qt::red, 2);

    for (auto it = _layouts.constBegin(); it != _layouts.constEnd(); ++it) {
        int id       = it.key();
        int parentId = it.value().node.getParentId();
        if (parentId < 0 || !_nodeItems.contains(parentId))
            continue;

        // Calculer le centre du parent
        QGraphicsEllipseItem *parentItem = _nodeItems[parentId];
        QPointF parentCenter = parentItem->rect().center() + parentItem->pos();

        // Calculer le centre de l'enfant
        QGraphicsEllipseItem *childItem = _nodeItems[id];
        QPointF childCenter = childItem->rect().center() + childItem->pos();

        _scene->addLine(QLineF(parentCenter, childCenter), edgePen)->setZValue(0);
    }

    // 7. Ajuster le zoom pour faire tenir tout l'arbre
    fitInView(_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void TreeGraphicsView::computeDepths()
{
    // Recherche en largeur (BFS) à partir des racines (parentId == -1)
    QQueue<int> queue;

    // Initialiser depth = 0 pour tous les nœuds dont parentId == -1, et les enfile
    for (auto it = _layouts.constBegin(); it != _layouts.constEnd(); ++it) {
        int id       = it.key();
        int parentId = it.value().node.getParentId();
        if (parentId < 0) {
            _layouts[id].depth = 0;
            queue.enqueue(id);
        }
    }

    // Pour chaque nœud sans parent enregistré, s'assurer qu'il existe dans _layouts
    // (si parentId miraculeusement absent de _layouts, l'ignorer)
    while (!queue.isEmpty()) {
        int currId = queue.dequeue();
        int currDepth = _layouts[currId].depth;

        // Pour chacun des enfants directs
        const QList<int> &children = _childrenMap.value(currId);
        for (int childId : children) {
            // On définit child.depth = currDepth + 1
            _layouts[childId].depth = currDepth + 1;
            queue.enqueue(childId);
        }
    }

    // Note : si des nœuds n'ont pas été atteints (cycle ou données invalides), ils garderont depth = -1.
    // On peut les forcer à depth = 0 pour éviter des problèmes ultérieurs.
    for (auto it = _layouts.begin(); it != _layouts.end(); ++it) {
        if (it.value().depth < 0)
            it.value().depth = 0;
    }
}

void TreeGraphicsView::assignPositions()
{
    // Démarrer la position courante en x à 0
    _currentX = 0.0;

    // Appeler positionRec pour chaque racine (parentId == -1)
    const QList<int> &roots = _childrenMap.value(-1);
    for (int rootId : roots) {
        positionRec(rootId);
    }
}

void TreeGraphicsView::positionRec(int nodeId)
{
    const QList<int> &children = _childrenMap.value(nodeId);

    if (children.isEmpty()) {
        // Feuille : on la place à currentX
        double x = _currentX;
        double y = _layouts[nodeId].depth * _vSpacing;
        _layouts[nodeId].pos = QPointF(x, y);

        // Incrémenter la position X pour la prochaine feuille
        _currentX += _hSpacing;
    }
    else {
        // Nœud interne : d'abord positionner tous les enfants
        for (int childId : children) {
            positionRec(childId);
        }

        // Récupérer x des enfants extrêmes
        double firstChildX = _layouts[children.first()].pos.x();
        double lastChildX  = _layouts[children.last()].pos.x();

        // Positionner ce nœud au milieu de ses enfants
        double x = (firstChildX + lastChildX) / 2.0;
        double y = _layouts[nodeId].depth * _vSpacing;
        _layouts[nodeId].pos = QPointF(x, y);
    }
}

void TreeGraphicsView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}
