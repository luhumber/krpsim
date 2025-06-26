#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include "QThread"

#include "TreeGraphicsView.h"
#include "BeamSearchWorker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow*         ui;

private slots:
    void on_FilesPushButtonClicked();
    void on_StartPushButtonClicked();
    void on_SolutionFound(const QVector<BeamNode>& nodes, const QVector<BeamNode>& solution);

signals:
    void signal_NodesVectorCreated(const QVector<BeamNode>& nodes, const QVector<BeamNode> &current_beam);
};
