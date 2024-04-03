/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *OpenBinFile;
    QPushButton *ReadBinFile;
    QPushButton *CloseBinFile;
    QListWidget *ShowBinaryFile;
    QPushButton *RefreshPorts;
    QPushButton *OpenPort;
    QComboBox *ListPorts;
    QPushButton *ClosePort;
    QPushButton *SendBinary;
    QProgressBar *SendingProgress;
    QPushButton *Exit;
    QListWidget *ShowSendingBinary;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1195, 443);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        OpenBinFile = new QPushButton(centralwidget);
        OpenBinFile->setObjectName("OpenBinFile");
        OpenBinFile->setGeometry(QRect(40, 30, 151, 31));
        ReadBinFile = new QPushButton(centralwidget);
        ReadBinFile->setObjectName("ReadBinFile");
        ReadBinFile->setGeometry(QRect(40, 70, 151, 31));
        CloseBinFile = new QPushButton(centralwidget);
        CloseBinFile->setObjectName("CloseBinFile");
        CloseBinFile->setGeometry(QRect(40, 110, 151, 31));
        ShowBinaryFile = new QListWidget(centralwidget);
        ShowBinaryFile->setObjectName("ShowBinaryFile");
        ShowBinaryFile->setGeometry(QRect(200, 30, 951, 111));
        RefreshPorts = new QPushButton(centralwidget);
        RefreshPorts->setObjectName("RefreshPorts");
        RefreshPorts->setGeometry(QRect(40, 160, 151, 31));
        OpenPort = new QPushButton(centralwidget);
        OpenPort->setObjectName("OpenPort");
        OpenPort->setGeometry(QRect(200, 160, 141, 31));
        ListPorts = new QComboBox(centralwidget);
        ListPorts->setObjectName("ListPorts");
        ListPorts->setGeometry(QRect(40, 200, 151, 31));
        ClosePort = new QPushButton(centralwidget);
        ClosePort->setObjectName("ClosePort");
        ClosePort->setGeometry(QRect(200, 200, 141, 31));
        SendBinary = new QPushButton(centralwidget);
        SendBinary->setObjectName("SendBinary");
        SendBinary->setGeometry(QRect(40, 250, 301, 31));
        SendingProgress = new QProgressBar(centralwidget);
        SendingProgress->setObjectName("SendingProgress");
        SendingProgress->setGeometry(QRect(40, 290, 301, 31));
        SendingProgress->setValue(24);
        Exit = new QPushButton(centralwidget);
        Exit->setObjectName("Exit");
        Exit->setGeometry(QRect(40, 340, 301, 31));
        ShowSendingBinary = new QListWidget(centralwidget);
        ShowSendingBinary->setObjectName("ShowSendingBinary");
        ShowSendingBinary->setGeometry(QRect(360, 160, 791, 211));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1195, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        OpenBinFile->setText(QCoreApplication::translate("MainWindow", "OpenBinFile", nullptr));
        ReadBinFile->setText(QCoreApplication::translate("MainWindow", "ReadBinFile", nullptr));
        CloseBinFile->setText(QCoreApplication::translate("MainWindow", "CloseBinFIle", nullptr));
        RefreshPorts->setText(QCoreApplication::translate("MainWindow", "RefreshPorts", nullptr));
        OpenPort->setText(QCoreApplication::translate("MainWindow", "OpenPort", nullptr));
        ClosePort->setText(QCoreApplication::translate("MainWindow", "ClosePort", nullptr));
        SendBinary->setText(QCoreApplication::translate("MainWindow", "SendBinary", nullptr));
        Exit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
