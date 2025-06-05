// TreeGraphicsView.cpp
#include "TreeGraphicsView.h"
#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QQueue>

TreeGraphicsView::TreeGraphicsView(QWidget *parent)
    : QGraphicsView(parent), _scene(new QGraphicsScene(this)), _current_x(0.0)
{
    setScene(_scene);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

void TreeGraphicsView::ClearData()
{
    _layouts.clear();
    _children_map.clear();
    _node_items.clear();
    _current_x = 0.0;
}

void TreeGraphicsView::on_NodesVectorCreated(const QVector<BeamNode> &nodes,
                                              const QVector<BeamNode> &current_beam)
{
    _scene->clear();
    this->ClearData();

    for (const BeamNode &bn : nodes) {
        int id = bn.getId();
        int parentId = bn.getParentId();
        _layouts[id] = { bn, -1, QPointF() };
        _children_map[parentId].append(id);
    }

    this->ComputeDepths();
    this->AssignPositions();

    QPen node_pen(QColor("#CCCCCC"));
    QBrush normal_brush(QColor("#222222"));
    QBrush solution_brush(QColor("#2ecc40"));

    QSet<int> beam_ids;
    for (const BeamNode &bn : current_beam) {
        beam_ids.insert(bn.getId());
    }

    for (auto it = _layouts.cbegin(); it != _layouts.cend(); ++it) {
        int id = it.key();
        const NodeLayout &nl = it.value();

        const QBrush &brush = beam_ids.contains(id) ? solution_brush : normal_brush;

        QString display_text = nl.node.getProcessName();
        QFont font;
        font.setPointSize(10);
        QFontMetrics fm(font);
        int text_width = fm.horizontalAdvance(display_text);
        int text_height = fm.height();

        double radius = std::max(_nodeRadius, std::max(text_width, text_height) / 2.0 + 10.0);

        double x = nl.pos.x() - radius;
        double y = nl.pos.y() - radius;

        QGraphicsEllipseItem *ellipse = _scene->addEllipse(
            x, y,
            radius * 2.0, radius * 2.0,
            node_pen, brush
        );
        ellipse->setZValue(1);
        ellipse->setToolTip(QString("Node %1: %2").arg(id).arg(display_text));
        _node_items[id] = ellipse;

        QGraphicsTextItem *text = _scene->addText(display_text);
        text->setFont(font);
        text->setDefaultTextColor(QColor("#EEEEEE"));
        QRectF ellipseRect = ellipse->rect();
        text->setPos(
            x + (radius * 2.0 - text->boundingRect().width()) / 2.0,
            y + (radius * 2.0 - text->boundingRect().height()) / 2.0
        );
        text->setZValue(2);
    }

    QPen edge_pen(QColor("#888888"), 2);

    for (auto it = _layouts.cbegin(); it != _layouts.cend(); ++it) {
        int id = it.key();
        int parentId = it.value().node.getParentId();
        if (parentId < 0 || !_node_items.contains(parentId))
            continue;

        QGraphicsEllipseItem *parentItem = _node_items[parentId];
        QPointF parentCenter = parentItem->rect().center() + parentItem->pos();

        QGraphicsEllipseItem *childItem = _node_items[id];
        QPointF childCenter = childItem->rect().center() + childItem->pos();

        _scene->addLine(QLineF(parentCenter, childCenter), edge_pen)->setZValue(0);
    }

    fitInView(_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void TreeGraphicsView::ComputeDepths()
{
    QQueue<int> queue;

    for (auto it = _layouts.cbegin(); it != _layouts.cend(); ++it) {
        int id = it.key();
        int parentId = it.value().node.getParentId();
        if (parentId < 0) {
            _layouts[id].depth = 0;
            queue.enqueue(id);
        }
    }

    while (!queue.isEmpty()) {
        int current_id = queue.dequeue();
        int current_depth = _layouts[current_id].depth;

        const QList<int> &children = _children_map.value(current_id);
        for (int child_id : children) {
            _layouts[child_id].depth = current_depth + 1;
            queue.enqueue(child_id);
        }
    }

    for (auto it = _layouts.begin(); it != _layouts.end(); ++it) {
        if (it.value().depth < 0)
            it.value().depth = 0;
    }
}

void TreeGraphicsView::AssignPositions()
{
    _current_x = 0.0;

    const QList<int> &roots = _children_map.value(-1);
    for (int rootId : roots) {
        this->PositionRec(rootId);
    }
}

void TreeGraphicsView::PositionRec(int nodeId)
{
    const QList<int> &children = _children_map.value(nodeId);

    if (children.isEmpty()) {
        double x = _current_x;
        double y = _layouts[nodeId].depth * _vSpacing;
        _layouts[nodeId].pos = QPointF(x, y);
        _current_x += _hSpacing;
    }
    else {
        for (int child_id : children) {
            this->PositionRec(child_id);
        }

        double firstChildX = _layouts[children.first()].pos.x();
        double lastChildX  = _layouts[children.last()].pos.x();

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
