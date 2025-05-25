#pragma once

#include <QGraphicsView>
#include <QGraphicsEllipseItem>

#include <QMap>
#include <QVector>

struct TreeNode {
    QString name;
    int id;
    QPointF center;
};

class TreeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TreeGraphicsView(QWidget *parent = nullptr);
    void CreateNode(const QString &name, int id, int parentId = -1);

protected:
    QGraphicsScene* _scene = nullptr;
    int             _nodeCount = 0;
    const int       _nodeSpacing = 90;
    const int       _nodeRadius = 20;
    QPointF         _last_center;

    QMap<int, TreeNode> _nodes;
    QMap<int, QVector<int>> _children;

    void CreateEdge(const QPointF &start, const QPointF &end);

    void wheelEvent(QWheelEvent *event) override;
};
