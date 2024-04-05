#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
