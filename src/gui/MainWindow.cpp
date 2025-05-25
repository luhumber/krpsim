#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Parser.h"
#include "Scenario.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->treeGraphicsView->setScene(scene);

    QPointF center1(20, 20);
    QPointF center2(120, 120);
    QGraphicsLineItem *line = scene->addLine(QLineF(center1, center2), QPen(Qt::black, 2));
    line->setZValue(0);

    QGraphicsEllipseItem *node1 = scene->addEllipse(0, 0, 40, 40, QPen(Qt::black), QBrush(Qt::yellow));
    QGraphicsEllipseItem *node2 = scene->addEllipse(100, 100, 40, 40, QPen(Qt::black), QBrush(Qt::yellow));
    node1->setZValue(1);
    node2->setZValue(1);

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
