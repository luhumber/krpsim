#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Parser.h"
#include "Scenario.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->filesPushButton, &QPushButton::clicked, this, &MainWindow::on_FilesPushButtonClicked);
    connect(ui->startPushButton, &QPushButton::clicked, this, &MainWindow::on_StartPushButtonClicked);

    connect(this, &MainWindow::signal_NodesVectorCreated, ui->treeGraphicsView, &TreeGraphicsView::on_NodesVectorCreated);
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
    try {
        Scenario scenario = Parser::ParseFile(std::filesystem::path(filePath.toStdString()));
        qDebug() << "Parsed scenario with resources:" << scenario.resources;
        BeamSearch beam_search(scenario, 2);
        beam_search.RunAlgorithm();
        emit signal_NodesVectorCreated(beam_search.getNodesVector(), beam_search.getSolutionPath());
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Parsing error"), e.what());
    }
}
