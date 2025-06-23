#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Parser.h"
#include "Scenario.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString filePath = QDir::current().filePath("../resources/pomme");
    Parser parser;
    try {
        Scenario scenario = parser.parseFile(std::filesystem::path(filePath.toStdString()));
        qDebug() << "Parsed scenario with resources:" << scenario.resources;
    } catch (const std::exception &e) {
        qDebug() << "Error parsing file:" << e.what();
    }

    connect(ui->filesPushButton, &QPushButton::clicked, this, &MainWindow::on_filesPushButtonClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_filesPushButtonClicked() {
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
            } else {
                QMessageBox::warning(
                    this,
                    tr("Invalid file format"),
                    tr("The selected file is not in the correct format (no extension expected).")
                );
            }
        }
    }
}
<<<<<<< Updated upstream
=======

void MainWindow::on_StartPushButtonClicked() {
    const QString filePath = ui->chosenFileLineEdit->text();
    if (!QFileInfo::exists(filePath)) {
        QMessageBox::warning(this, tr("File not found"), tr("The selected file does not exist."));
        return;
    }
    try {
        Scenario scenario = Parser::ParseFile(std::filesystem::path(filePath.toStdString()));
        qDebug() << "Parsed scenario with resources:" << scenario.resources;
        // for (const Process &process : scenario.processes) {
        //     qDebug() << "Process:" << process.name
        //              << "| Needs:" << process.needs.toString()
        //              << "| Results:" << process.results.toString()
        //              << "| Delay:" << process.delay;
        // }
        BeamSearch beam_search(scenario, 10);
        beam_search.RunAlgorithm();
        if (!trace_writer) {
            trace_writer = new TraceWriter();
        }
        trace_writer->writeTrace(beam_search.getSolutionPath());
        emit signal_NodesVectorCreated(beam_search.getNodesVector(), beam_search.getSolutionPath());
    } catch (const std::exception &e) {
        QMessageBox::critical(this, tr("Parsing error"), e.what());
    }
}
>>>>>>> Stashed changes
