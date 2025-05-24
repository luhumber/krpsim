/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *settingsGroupBox;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_4;
    QLabel *fileLabel;
    QLineEdit *chosenFileLineEdit;
    QPushButton *filesPushButton;
    QHBoxLayout *horizontalLayout_5;
    QLabel *delayLabel;
    QSpinBox *delaySpinBox;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(906, 694);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(-1, 0, -1, -1);
        settingsGroupBox = new QGroupBox(centralwidget);
        settingsGroupBox->setObjectName("settingsGroupBox");
        verticalLayout_7 = new QVBoxLayout(settingsGroupBox);
        verticalLayout_7->setObjectName("verticalLayout_7");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(-1, 0, -1, -1);
        fileLabel = new QLabel(settingsGroupBox);
        fileLabel->setObjectName("fileLabel");

        horizontalLayout_4->addWidget(fileLabel);

        chosenFileLineEdit = new QLineEdit(settingsGroupBox);
        chosenFileLineEdit->setObjectName("chosenFileLineEdit");
        chosenFileLineEdit->setEnabled(false);

        horizontalLayout_4->addWidget(chosenFileLineEdit);

        filesPushButton = new QPushButton(settingsGroupBox);
        filesPushButton->setObjectName("filesPushButton");

        horizontalLayout_4->addWidget(filesPushButton);


        verticalLayout_7->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(-1, 0, -1, -1);
        delayLabel = new QLabel(settingsGroupBox);
        delayLabel->setObjectName("delayLabel");

        horizontalLayout_5->addWidget(delayLabel);

        delaySpinBox = new QSpinBox(settingsGroupBox);
        delaySpinBox->setObjectName("delaySpinBox");

        horizontalLayout_5->addWidget(delaySpinBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_7->addLayout(horizontalLayout_5);


        horizontalLayout_3->addWidget(settingsGroupBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_3);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setTabPosition(QTabWidget::TabPosition::North);
        tabWidget->setTabShape(QTabWidget::TabShape::Rounded);
        tab = new QWidget();
        tab->setObjectName("tab");
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());

        verticalLayout_2->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 906, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        settingsGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        fileLabel->setText(QCoreApplication::translate("MainWindow", "File : ", nullptr));
        filesPushButton->setText(QCoreApplication::translate("MainWindow", "Files", nullptr));
        delayLabel->setText(QCoreApplication::translate("MainWindow", "Delay : ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
