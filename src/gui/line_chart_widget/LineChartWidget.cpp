#include "LineChartWidget.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QPalette>
#include <QDebug>

LineChartWidget::LineChartWidget(QWidget *parent)
    : QWidget(parent), m_series(nullptr), m_currentIndex(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#232629"));
    setAutoFillBackground(true);
    setPalette(pal);
}

void LineChartWidget::appendScore(double score)
{
    if (!m_series) {
        m_series = new QLineSeries();
        QChart *chart = new QChart();
        chart->addSeries(m_series);
        chart->setTitle("Beam Max Score per Iteration (Y) vs Iteration (X)");
        chart->createDefaultAxes();
        chart->setBackgroundBrush(QBrush(QColor("#232629")));
        chart->setTitleBrush(QBrush(Qt::white));
        chart->legend()->setVisible(false);

        QValueAxis *axisX = new QValueAxis;
        axisX->setTitleText("Iteration");
        chart->setAxisX(axisX, m_series);

        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText("Max Score of Beam");
        chart->setAxisY(axisY, m_series);

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setBackgroundBrush(QBrush(QColor("#232629")));
        chartView->setRubberBand(QChartView::RectangleRubberBand);
        chartView->setDragMode(QGraphicsView::ScrollHandDrag);

        QLayout *oldLayout = layout();
        if (oldLayout) {
            QLayoutItem *item;
            while ((item = oldLayout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
            delete oldLayout;
        }
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(chartView);
        setLayout(layout);
        m_currentIndex = 0;
    }
    m_series->append(m_currentIndex++, score);

    if (m_series->chart()) {
        m_series->chart()->axes(Qt::Horizontal).first()->setRange(0, m_currentIndex);
        const auto& points = m_series->pointsVector();
        if (!points.isEmpty()) {
            auto minMax = std::minmax_element(points.begin(), points.end(),
                                              [](const QPointF& a, const QPointF& b){ return a.y() < b.y(); });
            double minY = minMax.first->y();
            double maxY = minMax.second->y();
            m_series->chart()->axes(Qt::Vertical).first()->setRange(minY, maxY);
        }
    }
}
