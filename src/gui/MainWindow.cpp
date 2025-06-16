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

    connect(this, &MainWindow::signal_NodesVectorCreated,
        ui->treeGraphicsView, &TreeGraphicsView::on_NodesVectorCreated);
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

    try {
        Scenario scenario = Parser::ParseFile(std::filesystem::path(filePath.toStdString()));
        int beam_size = 10;

        QThread* thread = new QThread;
        BeamSearchWorker* worker = new BeamSearchWorker(scenario, beam_size);
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &BeamSearchWorker::process);
        connect(worker, &BeamSearchWorker::finished,
            this, [=](QVector<BeamNode> nodes, QVector<BeamNode> solution) {
                emit signal_NodesVectorCreated(nodes, solution);
                thread->quit();
        });
        connect(worker, &BeamSearchWorker::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        connect(worker, &BeamSearchWorker::scoreUpdated,
            ui->lineChartWidget, &LineChartWidget::appendScore);

        thread->start();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Parsing error"), e.what());
    }
}
