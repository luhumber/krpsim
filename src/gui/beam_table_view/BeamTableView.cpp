#include "BeamTableView.h"
#include <QStandardItemModel>
#include <QStringList>
#include <QHeaderView>
#include "BeamNode.h"

BeamTableView::BeamTableView(QWidget *parent) : QTableView(parent) {
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void BeamTableView::DisplaySolutionSteps(const QVector<BeamNode>& solution)
{
    QStandardItemModel* model = new QStandardItemModel(solution.size(), 4, this);
    model->setHorizontalHeaderLabels(QStringList() << "Processus" << "Stock" << "Temps" << "Score");

    for (int row = 0; row < solution.size(); ++row) {
        const BeamNode& node = solution[row];
        model->setItem(row, 0, new QStandardItem(node.getProcessName()));
        model->setItem(row, 1, new QStandardItem(node.getState().stock.toString()));
        model->setItem(row, 2, new QStandardItem(QString::number(node.getState().time)));
        model->setItem(row, 3, new QStandardItem(QString::number(node.score(), 'f', 2)));
    }
    setModel(model);
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void BeamTableView::ClearTable()
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(this->model());
    if (model) {
        model->clear();
        setModel(nullptr);
    }
}
