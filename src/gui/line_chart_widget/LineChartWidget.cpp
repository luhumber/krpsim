#include "LineChartWidget.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QVBoxLayout>
#include <QPalette>

LineChartWidget::LineChartWidget(QWidget *parent)
    : QWidget(parent)
{
    QSplineSeries *series = new QSplineSeries();
    series->append(0, 1);
    series->append(1, 3);
    series->append(2, 2);
    series->append(3, 5);
    series->append(4, 4);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Ligne simulée (arrondie)");
    chart->createDefaultAxes();

    chart->setBackgroundBrush(QBrush(QColor("#232629")));
    chart->setTitleBrush(QBrush(Qt::white));
    chart->legend()->setLabelColor(Qt::white);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setBackgroundBrush(QBrush(QColor("#232629")));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);

    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#232629"));
    setAutoFillBackground(true);
    setPalette(pal);
}
