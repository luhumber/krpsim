#include "BarChartWidget.h"
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QPalette>

BarChartWidget::BarChartWidget(QWidget *parent)
    : QWidget(parent)
{
    QBarSet *set = new QBarSet("Valeurs");
    *set << 1 << 3 << 2 << 5 << 4;
    set->setColor(QColor("#3498db"));

    QBarSeries *series = new QBarSeries();
    series->append(set);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Barres simulées");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->setBackgroundBrush(QBrush(QColor("#232629")));
    chart->setTitleBrush(QBrush(Qt::white));
    chart->legend()->setLabelColor(Qt::white);

    QStringList categories;
    categories << "A" << "B" << "C";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(Qt::white);
    chart->setAxisX(axisX, series);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelsColor(Qt::white);
    chart->setAxisY(axisY, series);

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
