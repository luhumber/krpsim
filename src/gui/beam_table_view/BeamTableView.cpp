#include "BeamTableView.h"
#include <QStandardItemModel>

BeamTableView::BeamTableView(QWidget *parent)
    : QTableView(parent)
{
    QStandardItemModel *model = new QStandardItemModel(5, 3, this);
    model->setHorizontalHeaderLabels(QStringList() << "Nom" << "Valeur" << "Statut");

    for (int row = 0; row < 5; ++row) {
        model->setItem(row, 0, new QStandardItem(QString("Item %1").arg(row + 1)));
        model->setItem(row, 1, new QStandardItem(QString::number((row + 1) * 10)));
        model->setItem(row, 2, new QStandardItem(row % 2 == 0 ? "OK" : "NOK"));
    }

    setModel(model);
}
