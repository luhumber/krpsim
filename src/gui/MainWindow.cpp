#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Parser.h"
#include "Scenario.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
