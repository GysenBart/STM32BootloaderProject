#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qlistwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QByteArray uintToLittleEndian(quint32 value);
    void readPendingDatagrams();
    void addLog(const QString &message);
    void saveListWidgetItemsToFile(QListWidget *listWidget, const QString &filePath);
    void handleReadyRead(QByteArray crc);
    QByteArray convertToQByteArray(unsigned short value);


private slots:
    void on_OpenBinFile_clicked();
    void on_ReadBinFile_clicked();
    void on_CloseBinFile_clicked();
    void on_RefreshPorts_clicked();
    void on_OpenPort_clicked();
    void on_ClosePort_clicked();
    void on_SendBinary_clicked();
    void on_SendingProgress_valueChanged(int value);
    void on_Exit_clicked();
    void on_ShowBinaryFile_currentTextChanged(const QString &currentText);
    void on_ListPorts_activated(int index);
    void on_EraseMem_clicked();
    void on_SendBinaryUDP_clicked(); 
    void on_LE_IPInput_returnPressed();
    void on_PB_EraseMemoryUDP_clicked();
    void on_LE_PortInput_returnPressed();
    void on_CB_LocalIp_activated(int index);
    void on_PB_Bind_clicked();
    void on_PB_Unbind_clicked();
    void on_PB_SaveLog_clicked();

private:
    Ui::MainWindow *ui;
    void initSocket(QString addr);
    void waitForBytes(void);

};
#endif // MAINWINDOW_H
