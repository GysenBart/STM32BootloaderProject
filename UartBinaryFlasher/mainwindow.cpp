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
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QNetworkInterface>
#include <QStandardPaths>
#include "crc.h"

#define UserAppSize 10
#define UDPsend 1
#define UDPwait 2
#define EOT 5
#define UDPerase 4
#define SERIALsend 1
#define SERIALwait 2
#define SERIALerase 4
#define StartAddrMASK 0x08020000

qint8 WriteCommand = 0x57;
bool IsBinPresent = false;
QFile Binary;
QByteArray Payload;
qint64 binLen = 0;
QString SelectedComPort;
QString IPaddr;
QString LocalIP;
int UDP_Port;
QSerialPort serial;
bool ComportAvailable = false;
const char ErasePacket[5] = {0x04, 0x56, 0x05, 0x10, 0};
const char ErasePacketUDP[4] = {0x56, 0x05, 0x10, 0};
// End of transmission packet
const char EotPacket[6] = {0x45, 0x4F, 0x54, 0xA, 0xD, 0}; //"EOT\n\r"
bool binReady = false;
bool ipReady = false;
bool UDP_packetRecvOk = false;
bool UDP_eraseRecvOk = false;
bool Serial_CrcCompareOk = false;
int UDPcount;

QByteArray crcResponse;
QByteArray CrcArray;

QUdpSocket *udpSocket;
crc *crcCalculator;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("STM32F429ZI flash program");
    //initSocket();
    // set the name to the button
    ui->PB_EraseMemoryUDP->setText("EraseMemory");
    ui->SendBinaryUDP->setText("SendBinary");
    ui->PB_SaveLog->setText("Save logging");
    // set input restrictions for Port to int
    ui->LE_PortInput->setValidator(new QIntValidator(0, 99999, this));
    // set name to the Portbox
    ui->LE_PortInput->setText("Enter Port");
    // set bind boxes
    ui->PB_Bind->setText("Bind");
    ui->PB_Unbind->setText("Unbind");

    // disable buttons
    ui->LE_IPInput->setDisabled(true);
    ui->PB_EraseMemoryUDP->setDisabled(true);
    ui->SendBinaryUDP->setDisabled(true);
    ui->SendBinary->setDisabled(true);
    ui->EraseMem->setDisabled(true);

    ui->LE_IPInput->setText("192.168.000.101");
    ui->LE_PortInput->setText("7755");

    // Set the value of the process bar to 0
    ui->SendingProgress->setValue(0);

    udpSocket = new QUdpSocket(this);

    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
            ui->CB_LocalIp->addItem(address.toString());
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initSocket(QString addr)
{

    udpSocket->bind(QHostAddress(addr), 7755);
    //udpSocket->bind(QHostAddress::LocalHost, 7755);

    connect(udpSocket, &QUdpSocket::readyRead,
            this, &MainWindow::readPendingDatagrams, Qt::QueuedConnection);

    ui->PB_EraseMemoryUDP->setEnabled(true);
}

void MainWindow::on_OpenBinFile_clicked()
{
    QString initialDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filter = "Binary Files (*.bin)";
    QString path = QFileDialog::getOpenFileName(this, "Open File", initialDir, filter);
    if(!path.isEmpty())
    {
        ui->ShowBinaryFile->clear();
        IsBinPresent = true;
        Binary.setFileName(path);
        if(!Binary.open(QIODevice::ReadOnly))
        {
            addLog("Failed to open file for reading.");
            qDebug() << "Failed to open file for reading";
        }
        else
        {
            addLog("Opened file: " + path);
        }
    }
    else
    {
        addLog("No file selected.");
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
                addLog("Failed to read data from file");
                qDebug() << "Failed to read data from file";
            }
            Payload.append(buffer, bytesRead);
        }
        binLen = Payload.size();
        QString strLen = QString::number(binLen);
        ui->ShowBinaryFile->addItem("The length of the binary:");
        ui->ShowBinaryFile->addItem(strLen);
        addLog("Binary readed.");

        if (ipReady)
        {
            ui->SendBinaryUDP->setEnabled(true);
        }
        else
        {
            binReady = true;
        }
    }
    else
    {
        addLog("No binary file available to read.");
    }
}

void MainWindow::on_CloseBinFile_clicked()
{
    Binary.close();
    addLog("Binary closed.");
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
    // use index instead of currentText!
    SelectedComPort = ui->ListPorts->itemText(index);
    serial.setPortName(SelectedComPort);
    ComportAvailable = true;
    addLog("SERIAL: Selected " + SelectedComPort);
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
        if(serial.open(QIODevice::ReadWrite))
        {
            addLog("SERIAL: COM opened");
        }
        else
        {
            addLog("SERIAL: Failed to open COM");
        }
    }
    else
    {
        addLog("SERIAL: No COM selected");
    }
    ui->SendBinary->setEnabled(true);
    ui->EraseMem->setEnabled(true);
}

void MainWindow::on_ClosePort_clicked()
{
    serial.close();
    addLog("SERIAL: COM closed.");
    ui->SendBinary->setDisabled(true);
    ui->EraseMem->setDisabled(true);
}

// Function to convert a uint32 value to a little-endian byte array
QByteArray MainWindow::uintToLittleEndian(quint32 value) {
    QByteArray byteArray;
    byteArray.append((value) & 0xFF);
    byteArray.append((value >> 8) & 0xFF);
    byteArray.append((value >> 16) & 0xFF);
    byteArray.append((value >> 24) & 0xFF);
    return byteArray;
}

void MainWindow::on_SendBinary_clicked()
{
    ui->SendBinary->setText("busy");
    ui->SendBinary->setDisabled(true);
    ui->EraseMem->setDisabled(true);

    int i = 0;
    int j;
    int count = 1;
    uint16_t indexMASK = 50;
    uint8_t lenToAdd = 6;
    uint8_t PayLen = indexMASK;
    uint8_t totalLen = lenToAdd + PayLen;
    QByteArray packetArray;
    QByteArray addrArray;
    QString countString;
    quint16 calculatedCRC;
    quint32 address = StartAddrMASK;
    int binIndex = indexMASK;
    int progressRange = binLen/binIndex;
    bool sending = true;
    int progresState = 1;
    int SERIALcount = 0;

    ui->SendingProgress->setRange(0, progressRange);

    for(i = 0; i < binLen; i += PayLen)
    {
        packetArray.clear();
        packetArray.append(totalLen);
        packetArray.append(WriteCommand);
        addrArray = uintToLittleEndian(address);
        packetArray.append(addrArray);
        packetArray.append(PayLen);

        for(j = i; j < binIndex && j < binLen; j++)
        {
            packetArray.append(Payload[j]);
        }

        serial.write(packetArray);
        serial.flush();
        QThread::msleep(100);
        serial.waitForBytesWritten(100);
        binIndex += indexMASK;
        address += indexMASK;

        countString = QString::number(count);
        addLog("SERIAL: packet sending count: " + countString);
        ui->SendingProgress->setValue(count);
        count++;

        calculatedCRC = crcCalculator->CRC16(&packetArray, packetArray[0]);
        addLog("CRC16 checksum: " + QString::number(calculatedCRC, 16).toUpper());
    }
    QThread::msleep(100);

    packetArray.clear();
    packetArray.append(6);
    packetArray.append(EotPacket);
    serial.write(packetArray);
    serial.flush();
    QThread::msleep(100);
    serial.waitForBytesWritten(100);
    addLog("SERIAL: end of transmission");



    // while(sending)
    // {
    //     switch (progresState)
    //     {
    //     case SERIALsend:
    //         if(i < binLen)
    //         {
    //             packetArray.clear();
    //             packetArray.append(totalLen);
    //             packetArray.append(WriteCommand);
    //             addrArray = uintToLittleEndian(address);
    //             packetArray.append(addrArray);
    //             packetArray.append(PayLen);

    //             for(j = i; j < binIndex && j < binLen; j++)
    //             {
    //                 packetArray.append(Payload[j]);
    //             }

    //             binIndex += indexMASK;
    //             address += indexMASK;

    //             serial.write(packetArray);
    //             serial.flush();
    //             QThread::msleep(50);
    //             //serial.waitForBytesWritten(100);
    //             binIndex += indexMASK;
    //             address += indexMASK;

    //             CrcArray = convertToQByteArray(crcCalculator->CRC16(&packetArray, packetArray[0]));

    //             countString = QString::number(SERIALcount);
    //             addLog("SERIAL: packet sending count: " + countString);
    //             ui->SendingProgress->setValue(SERIALcount);
    //             SERIALcount++;

    //             i += PayLen;
    //         }
    //         progresState = UDPwait;

    //         break;

    //     case SERIALwait:
    //         handleReadyRead(CrcArray);
    //         if(Serial_CrcCompareOk == true)
    //         {
    //             count = 0;
    //             Serial_CrcCompareOk = false;
    //             if(j == binLen)
    //             {
    //                 // last bytes are sended
    //                 progresState = EOT;
    //             }
    //             else
    //             {
    //                 progresState = SERIALsend;
    //             }
    //             break;
    //         }
    //         else if (count > 50)
    //         {
    //             count = 0;
    //             sending = false;
    //             addLog("SERIAL: something went wrong while sending the packet");
    //             break;
    //         }
    //         count++;
    //         break;

    //     case EOT:
    //          packetArray.clear();
    //          packetArray.append(6);
    //          packetArray.append(EotPacket);
    //          serial.write(packetArray);
    //          addLog("SERIAL: end of transmission");
    //          sending = false;
    //          break;

    //     default:
    //         break;
    //     }
        //QThread::msleep(250);
    //}


    ui->SendBinary->setText("SendBinary");
    ui->SendBinary->setEnabled(true);
    ui->EraseMem->setEnabled(true);
}

void MainWindow::handleReadyRead(QByteArray crc)
{
    const int bufferSize = 10;
    QByteArray datacrc;
    QString temp;
    int bytesRead;
    char buffer[bufferSize];

    // while(serial.bytesAvailable())
    //{
    // if(serial.waitForReadyRead(1000))
    // {
            // data.append(serial.readAll());
        bytesRead = serial.read(buffer, bufferSize);
        if(bytesRead > 0)
        {
            QByteArray data(buffer, bytesRead);
            datacrc.append(data);
            if((datacrc[0] == crc[0])&&(datacrc[1] == crc[1]))
            {
                //serial.clear(QSerialPort::AllDirections);
                Serial_CrcCompareOk = true;
                datacrc.clear();
                addLog("SERIAL: Data received: crc compared ok");
                //break;
            }
        }
        else
        {
            addLog("SERIAL: something went wrong with reading data");
        }
    // }
    QThread::msleep(100);
}

QByteArray MainWindow::convertToQByteArray(unsigned short value)
{
    QByteArray byteArray;
    byteArray.append(static_cast<char>(value & 0x00FF));        // Low byte
    byteArray.append(static_cast<char>((value & 0xFF00) >> 8)); // High byte
    return byteArray;
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

void MainWindow::on_EraseMem_clicked()
{
    QByteArray eraseArray;
    eraseArray.append(ErasePacket);
    serial.write(eraseArray);
    addLog("SERIAL: erasing memory.");

}

void MainWindow::on_SendBinaryUDP_clicked()
{
    ui->SendBinaryUDP->setText("Busy");
    ui->SendBinaryUDP->setDisabled(true);

    int i = 0;
    int j;
    int count = 0;
    UDPcount = 1;
    uint16_t indexMASK = 150;
    //uint16_t PayLen = 250;
    uint16_t PayLen = indexMASK;
    QByteArray packetArray;
    QByteArray addrArray;
    QString countString;
    bool sending = true;
    int progresState = 1;
    quint32 address = StartAddrMASK;
    int binIndex = indexMASK;
    int progressRange = binLen/binIndex;

    ui->SendingProgress->setRange(0, progressRange);

    addLog("UDP: Start to send binary file.");

    while(sending)
    {
        switch (progresState)
        {
        case UDPsend:
            if(i < binLen)
            {
                packetArray.clear();
                packetArray.append(WriteCommand);
                addrArray = uintToLittleEndian(address);
                packetArray.append(addrArray);
                packetArray.append(PayLen);

                for(j = i; j < binIndex && j < binLen; j++)
                {
                    packetArray.append(Payload[j]);
                }

                binIndex += indexMASK;
                address += indexMASK;

                QByteArray Data;
                Data.append(packetArray);
                udpSocket->writeDatagram(Data, QHostAddress(IPaddr), UDP_Port);
                QThread::msleep(100);

                countString = QString::number(UDPcount);
                addLog("UDP: packet sending count: " + countString);
                ui->SendingProgress->setValue(UDPcount);
                UDPcount++;

                i += PayLen;
            }
            progresState = UDPwait;

            break;

        case UDPwait:
            readPendingDatagrams();
            if(UDP_packetRecvOk == true)
            {
                count = 0;                
                UDP_packetRecvOk = false;
                if(j == binLen)
                {
                    // last bytes are sended
                    progresState = EOT;
                }
                else
                {
                   progresState = UDPsend;
                }
                break;
            }
            else if (count > 50)
            {
                count = 0;
                sending = false;
                addLog("UDP: something went wrong while sending the packet");
                break;
            }
            count++;
            break;

        case EOT:
            packetArray.clear();
            packetArray.append(EotPacket);
            udpSocket->writeDatagram(packetArray, QHostAddress(IPaddr), UDP_Port);
            sending = false;
            break;

        default:
            break;
        }
        QThread::msleep(50);
    }

    ui->SendBinaryUDP->setText("SendBinary");
    ui->SendBinaryUDP->setEnabled(true);
}

void MainWindow::readPendingDatagrams()
{
    //ui->PB_EraseMemoryUDP->setDisabled(true);
    //ui->SendBinaryUDP->setDisabled(true);
    while (udpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        // If erase command received, disable udp buttons
        QByteArray replyData = datagram.data();

        if (replyData[0] == 0x57)
        {
            if (replyData[1] == 0x00)
            {
                UDP_packetRecvOk = true;
                addLog("UDP: succesfully received packet.");
            }
            else
            {
                // FOUTMELDING GEVEN!
                addLog("UDP: sending packet failed.");
            }
        }

        if (replyData[0] == 0x56)
        {
            if (replyData[1] == 0x00)
            {
                UDP_eraseRecvOk = true;
                addLog("UDP: memory succesful erased.");
            }
            else
            {
                addLog("UDP: erase memory failed.");
            }
        }
    }
}

void MainWindow::on_LE_IPInput_returnPressed()
{
    IPaddr = ui->LE_IPInput->text();
//    initSocket(IPaddr);
    ui->LE_IPInput->setStyleSheet("background-color: green");

    addLog("Destination IP set to: " + IPaddr);

    if (binReady)
    {
        ui->SendBinaryUDP->setEnabled(true);
    }
    else
    {
        ipReady = true;
    }
}

void MainWindow::on_PB_EraseMemoryUDP_clicked()
{
    ui->PB_EraseMemoryUDP->setText("busy");
    ui->PB_EraseMemoryUDP->setDisabled(true);

    addLog("UDP: erasing memory.");

    int eraseState = UDPerase;
    int count = 0;
    bool erasing = true;
    QByteArray EraseData;
    EraseData.append(ErasePacketUDP);
    // udpSocket->writeDatagram(EraseData, QHostAddress(IPaddr), UDP_Port);
    //QThread::msleep(3000);

    while(erasing)
    {
        switch (eraseState)
        {
        case UDPerase:
            udpSocket->writeDatagram(EraseData, QHostAddress(IPaddr), UDP_Port);
            eraseState = UDPwait;
            break;

        case UDPwait:
            readPendingDatagrams();
            if(UDP_eraseRecvOk)
            {
                count = 0;
                erasing = false;
                UDP_eraseRecvOk = false;
                break;
            }
            else if(count > 25)
            {
                count = 0;
                erasing = false;
                addLog("UDP: something went wrong while erasing memory");
                break;
            }
            count++;
            break;

        default:
            break;
        }

        QThread::msleep(1000);
    }

    ui->PB_EraseMemoryUDP->setEnabled(true);
    ui->PB_EraseMemoryUDP->setText("EraseMemory");
}

void MainWindow::on_LE_PortInput_returnPressed()
{
    QString STRING_NUM = ui->LE_PortInput->text();
    UDP_Port = STRING_NUM.toInt();
    ui->LE_PortInput->setStyleSheet("background-color: green");
    ui->LE_IPInput->setEnabled(true);
    addLog("UDP: selected port: " + STRING_NUM);
}

void MainWindow::on_CB_LocalIp_activated(int index)
{
    LocalIP = ui->CB_LocalIp->itemText(index);
    addLog("Set host ip to: "+ LocalIP);
}

void MainWindow::on_PB_Bind_clicked()
{
    initSocket(LocalIP);
    ui->CB_LocalIp->setDisabled(true);
    ui->LE_IPInput->setEnabled(true);
    addLog("UDP: binded.");
}

void MainWindow::on_PB_Unbind_clicked()
{
    udpSocket->close();
    ui->CB_LocalIp->setEnabled(true);
    addLog("UDP: unbinded.");
}

void MainWindow::addLog(const QString &message)
{
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString log = QString("[%1] %2").arg(currentTime, message);
    ui->LogWindow->addItem(log);
    ui->LogWindow->scrollToBottom();
    QApplication::processEvents();
}

void MainWindow::saveListWidgetItemsToFile(QListWidget *listWidget, const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        addLog("Could not open file for writing: " + filePath);
        //qDebug() << "Could not open file for writing:" << filePath;
        return;
    }

    QTextStream out(&file);
    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem *item = listWidget->item(i);
        out << item->text() << "\n";
    }

    file.close();
    addLog("Items saved to: " + filePath);
    //qDebug() << "Items saved to" << filePath;
}

void MainWindow::on_PB_SaveLog_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save to File", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), "Text Files (*.txt)");
    if (!filePath.isEmpty()) {
        saveListWidgetItemsToFile(ui->LogWindow, filePath);
    }
}
