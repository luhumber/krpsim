#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include "QThread"

#include "TreeGraphicsView.h"
#include "BeamSearchWorker.h"
#include "BeamSearch.h"
#include "TraceWriter.h"

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
    QThread*                _current_thread = nullptr;
    bool                    _search_interrupted = false;

private slots:
    void on_FilesPushButtonClicked();
    void on_StartPushButtonClicked();
    void on_StopPushButtonClicked();
    void on_SolutionFound(const QVector<BeamNode>& nodes, const QVector<BeamNode>& solution);
    void on_BeamSearchFinished(QVector<BeamNode> nodes, QVector<BeamNode> solution);

signals:
    void signal_NodesVectorCreated(const QVector<BeamNode>& nodes,
        const QVector<BeamNode> &current_beam);
};
