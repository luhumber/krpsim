#include "TreeGraphicsView.h"
#include <QWheelEvent>

TreeGraphicsView::TreeGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    _scene = new QGraphicsScene(this);
    setScene(_scene);

    setDragMode(QGraphicsView::ScrollHandDrag);

    // Racine
    this->CreateNode("Root", 1);

    // Niveau 1
    this->CreateNode("A", 2, 1);
    this->CreateNode("B", 3, 1);
    this->CreateNode("C", 4, 1);
    this->CreateNode("D", 5, 1);

    // Niveau 2
    this->CreateNode("A1", 6, 2);
    this->CreateNode("A2", 7, 2);
    this->CreateNode("B1", 8, 3);
    this->CreateNode("B2", 9, 3);
    this->CreateNode("B3", 10, 3);
    this->CreateNode("C1", 11, 4);

    // Niveau 3
    this->CreateNode("A1a", 12, 6);
    this->CreateNode("A1b", 13, 6);
    this->CreateNode("A2a", 14, 7);
    this->CreateNode("B1a", 15, 8);
    this->CreateNode("B3a", 16, 10);
    this->CreateNode("B3b", 17, 10);
    this->CreateNode("C1a", 18, 11);
    this->CreateNode("C1b", 19, 11);
    this->CreateNode("C1c", 20, 11);

    // Niveau 4
    this->CreateNode("A1a1", 21, 12);
    this->CreateNode("A1a2", 22, 12);
    this->CreateNode("B3a1", 23, 16);
    this->CreateNode("C1b1", 24, 19);
    this->CreateNode("C1b2", 25, 19);

    // Niveau 5
    this->CreateNode("C1b1a", 26, 24);
    this->CreateNode("C1b1b", 27, 24);
    this->CreateNode("C1b2a", 28, 25);

    // Branche supplémentaire sur D
    this->CreateNode("D1", 29, 5);
    this->CreateNode("D2", 30, 5);
    this->CreateNode("D3", 31, 5);
    this->CreateNode("D1a", 32, 29);
    this->CreateNode("D1b", 33, 29);
    this->CreateNode("D2a", 34, 30);
    this->CreateNode("D3a", 35, 31);
    this->CreateNode("D3b", 36, 31);
    this->CreateNode("D3c", 37, 31);
}

void TreeGraphicsView::CreateNode(const QString &name, int id, int parentId)
{
    QPointF center;
    if (parentId == -1) {
        center = QPointF(0, 0);
    } else {
        int siblingCount = _children[parentId].size();
        double verticalSpacing = _nodeRadius * 5;
        center = _nodes[parentId].center + QPointF(_nodeSpacing * 2, (siblingCount - 0.5) * verticalSpacing);
        _children[parentId].append(id);
    }

    QGraphicsEllipseItem *node = _scene->addEllipse(center.x() - _nodeRadius, center.y() - _nodeRadius, _nodeRadius * 2, _nodeRadius * 2, QPen(Qt::black), QBrush(Qt::yellow));
    node->setZValue(1);
    node->setToolTip(QString("Node %1: %2").arg(id).arg(name));

    _nodes[id] = {name, id, center};

    if (parentId != -1)
        this->CreateEdge(center, _nodes[parentId].center);

    _nodeCount++;
}

void TreeGraphicsView::CreateEdge(const QPointF &start, const QPointF &end)
{
    QGraphicsLineItem *edge = _scene->addLine(QLineF(start, end), QPen(Qt::red, 2));
    edge->setZValue(0);
}

void TreeGraphicsView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}
