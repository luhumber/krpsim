#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include "TreeGraphicsView.h"
<<<<<<< Updated upstream
=======
#include "BeamSearch.h"
#include "TraceWriter.h"
>>>>>>> Stashed changes

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
    TraceWriter*            trace_writer = nullptr;

private slots:
    void on_filesPushButtonClicked();
};
