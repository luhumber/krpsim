#pragma once

#include <QGraphicsView>

class TreeGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TreeGraphicsView(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
};
