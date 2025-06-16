#pragma once

#include <QWidget>
#include <QLineSeries>

class LineChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LineChartWidget(QWidget *parent = nullptr);

public slots:
    void appendScore(double score);

private:
    QLineSeries* m_series = nullptr;
    int m_currentIndex = 0;
};