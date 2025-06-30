#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Parser.h"
#include "Scenario.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->filesPushButton, &QPushButton::clicked,
        this, &MainWindow::on_FilesPushButtonClicked);
    connect(ui->startPushButton, &QPushButton::clicked,
        this, &MainWindow::on_StartPushButtonClicked);
    connect(ui->stopPushButton, &QPushButton::clicked,
        this, &MainWindow::on_StopPushButtonClicked);

    connect(this, &MainWindow::signal_NodesVectorCreated,
        ui->treeGraphicsView, &TreeGraphicsView::on_NodesVectorCreated);
    connect(this, &MainWindow::signal_NodesVectorCreated,
        this, &MainWindow::on_SolutionFound);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_FilesPushButtonClicked() {
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Select a file"),
        QDir::homePath(),
        tr("All files (*)")
    );
    if (!fileNames.isEmpty()) {
        for (const QString& fileName : fileNames) {
            QFileInfo info(fileName);
            if (info.suffix().isEmpty()) {
                qDebug() << "Selected file:" << fileName;
                ui->chosenFileLineEdit->setText(fileName);
                ui->startPushButton->setEnabled(true);
            } else {
                QMessageBox::warning(
                    this,
                    tr("Invalid file format"),
                    tr("The selected file is not in the correct format (no extension expected).")
                );
                ui->chosenFileLineEdit->clear();
                ui->startPushButton->setEnabled(false);
            }
        }
    }
}

void MainWindow::on_StartPushButtonClicked() {
    const QString filePath = ui->chosenFileLineEdit->text();
    if (!QFileInfo::exists(filePath)) {
        QMessageBox::warning(this, tr("File not found"), tr("The selected file does not exist."));
        return;
    }
    ui->startPushButton->setEnabled(false);
    ui->filesPushButton->setEnabled(false);
    ui->stopPushButton->setEnabled(true);
    _search_interrupted = false;

    try {
        Scenario scenario = Parser::ParseFile(std::filesystem::path(filePath.toStdString()));
        int beam_size = 10;

        QThread* thread = new QThread;
        _current_thread = thread;
        BeamSearchWorker* worker = new BeamSearchWorker(scenario,
            beam_size, ui->delaySpinBox->value());
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &BeamSearchWorker::process);
        connect(worker, &BeamSearchWorker::finished,
            this, [=](QVector<BeamNode> nodes, QVector<BeamNode> solution) {
                if (!_search_interrupted) {
                    emit signal_NodesVectorCreated(nodes, solution);
                } else {
                    ui->beamTableView->ClearTable();
                    ui->treeGraphicsView->ClearTree();
                }
                _current_thread = nullptr;
                thread->quit();
        });
        connect(worker, &BeamSearchWorker::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        thread->start();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Parsing error"), e.what());
    }
}

void MainWindow::on_StopPushButtonClicked() {
    _search_interrupted = true;
    
    if (_current_thread && _current_thread->isRunning()) {
        _current_thread->requestInterruption();
        _current_thread->quit();
        if (!_current_thread->wait(5000)) {
            _current_thread->terminate();
            _current_thread->wait();
        }
        _current_thread = nullptr;
    }
    
    ui->startPushButton->setEnabled(true);
    ui->filesPushButton->setEnabled(true);
    ui->stopPushButton->setEnabled(false);
    ui->tabWidget->setEnabled(false);

    QMessageBox::information(this, tr("Beam Search"), tr("Beam search has been stopped."));
}

void MainWindow::on_SolutionFound(const QVector<BeamNode>& nodes, const QVector<BeamNode>& solution) {
    ui->tabWidget->setEnabled(true);
    ui->filesPushButton->setEnabled(true);
    ui->stopPushButton->setEnabled(false);
    ui->beamTableView->DisplaySolutionSteps(solution);
}
