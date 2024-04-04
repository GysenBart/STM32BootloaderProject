#include "mainwindow.h"
//#include "Error in " //Util.relativeFilePath('D:/Bart/School/IoT 3 ' 23 - '24/Embedded Devices Advanced/mainwindow.h', 'D:/Bart/School/IoT 3 ' 23 - '24/Embedded Devices Advanced' + '/' + Util.path('mainwindow.cpp')) "//: SyntaxError: Expected token `)'"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QThread>


#define UserAppSize 10
//#define WriteCommand 0x57

qint8 WriteCommand = 0x57;
bool IsBinPresent = false;
QFile Binary;
const char StartAddr[4] = {0x00, 0x00, 0x01, 0x08};
QByteArray dataArray;
qint64 binLen = 0;
QString SelectedComPort;
QSerialPort serial;
bool ComportAvailable = false;
const char ErasePacket[4] = {0x04, 0x56, 0x04, 0x10};

// Function to increment the address in a QByteArray
void incrementAddress(QByteArray &packet, int incrementStart, int incrementEnd, int increment) {
    // Increment the address bytes by the specified value
    int carry = increment;
    for (int i = incrementStart; i <= incrementEnd && carry > 0; ++i) {
        int sum = packet[i] + carry;
        packet[i] = sum & 0xFF; // Keep only the least significant byte
        carry = sum >> 8; // Update the carry with the carry-over bits
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenBinFile_clicked()
{
    QString initialDir = "D:/Bart/School/IoT 3 '23 - '24/Embedded Devices Advanced";
    QString filter = "Binary Files (*.bin)";
    QString path = QFileDialog::getOpenFileName(this, "Open File", initialDir, filter);
    QString NoFile = "No file selected";
    if(!path.isEmpty())
    {
        ui->ShowBinaryFile->clear();
        IsBinPresent = true;
        Binary.setFileName(path);
        if(!Binary.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open file for reading";
        }
    }
    else
    {
        ui->ShowBinaryFile->addItem(NoFile);
        ui->ShowBinaryFile->show();
    }
}


void MainWindow::on_ReadBinFile_clicked()
{
    if(IsBinPresent)
    {
        QDataStream in(&Binary);
        char buffer[UserAppSize];
        while(!in.atEnd())
        {
            qint64 bytesRead = in.readRawData(buffer, sizeof(buffer));
            if(bytesRead == -1)
            {
                qDebug() << "Failed to read data from file";
            }
            dataArray.append(buffer, bytesRead);
        }
        binLen = dataArray.size();
        QString strLen = QString::number(binLen);
        ui->ShowBinaryFile->addItem("The length of the binary:");
        ui->ShowBinaryFile->addItem(strLen);
        // ui->ShowBinaryFile->addItem("binary:");
        // ui->ShowBinaryFile->addItem(buffer);
        ui->ShowBinaryFile->show();
    }
}


void MainWindow::on_CloseBinFile_clicked()
{
    Binary.close();
}


void MainWindow::on_RefreshPorts_clicked()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &PortInfo : ports)
    {
        ui->ListPorts->clear();
        ui->ListPorts->addItem(PortInfo.portName());
        ui->ListPorts->show();
    }
}


void MainWindow::on_ListPorts_activated(int index)
{
    SelectedComPort = ui->ListPorts->currentText();
    serial.setPortName(SelectedComPort);
    ComportAvailable = true;
}


void MainWindow::on_OpenPort_clicked()
{
    if(ComportAvailable)
    {
        serial.setBaudRate(QSerialPort::Baud115200);
        serial.setDataBits(QSerialPort::Data8);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        if(serial.open(QIODevice::WriteOnly))
        {
            ui->ShowSendingBinary->clear();
            ui->ShowSendingBinary->addItem("COM opened\n");
            ui->ShowSendingBinary->show();
        }
        else
        {
            ui->ShowSendingBinary->clear();
            ui->ShowSendingBinary->addItem("Failed to open COM\n");
            ui->ShowSendingBinary->show();
        }
    }
    else
    {
        ui->ShowSendingBinary->clear();
        ui->ShowSendingBinary->addItem("No COM selected");
        ui->ShowSendingBinary->show();
    }
}


void MainWindow::on_ClosePort_clicked()
{
    serial.close();
}


void MainWindow::on_SendBinary_clicked()
{
    int i;
    qint8 lenToAdd = 6;
    qint64 testlen = binLen;
//    qint64 totalLen = lenToAdd + binLen;
    qint64 totalLen = lenToAdd + testlen;
    QByteArray packetArray;
    packetArray.append(totalLen);
    packetArray.append(WriteCommand);
    QByteArray eraseArray;
    eraseArray.append(ErasePacket);
    const int incrementStart = 2;
    const int incrementEnd = 5;
    const int incrementAmount = 0xFF;

    serial.write(eraseArray);
    QThread::msleep(100);

    for(i = 0; i < sizeof(StartAddr); i++)
    {
        packetArray.append(StartAddr[i]);
    }

//    packetArray.append(binLen);

    packetArray.append(testlen);

    for(i = 0; i < testlen; i++)
    {
        packetArray.append(dataArray[i]);
    }
    /* What i have to do here:
       every 255 times
       change to totel length
       set the write command
       change the start address by incrementing with 0xFF
       change the payloadlength*/
    for(i = 0; i < packetArray.size(); i += 255)
    {
        QByteArray splitPacket = packetArray.mid(i, 255);
        serial.write(splitPacket);
        incrementAddress(packetArray, incrementStart, incrementEnd, incrementAmount);
        QThread::msleep(10);

    }

   // serial.write(packetArray);
}


void MainWindow::on_SendingProgress_valueChanged(int value)
{

}


void MainWindow::on_Exit_clicked()
{
    Binary.close();
    serial.close();
    QCoreApplication::exit();
}


void MainWindow::on_ShowBinaryFile_currentTextChanged(const QString &currentText)
{

}




