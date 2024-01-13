#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QTime>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QDate>
#include <QTextStream>
#include <QFile>
#include <QString>
class SerialPortSetting;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setConnections();


private:
    void UpdateChartAndSaveData();
private slots:
    void on_OpenBtn_clicked();

    void on_StartButton_clicked();
    void on_Channel1Box_stateChanged(int arg1);

    void on_Channel2Box_stateChanged(int arg1);

    void on_Channel3Box_stateChanged(int arg1);

    void on_Channel4Box_stateChanged(int arg1);

    void on_Channel5Box_stateChanged(int arg1);

    void on_Channel6Box_stateChanged(int arg1);

    void on_Channel7Box_stateChanged(int arg1);

    void on_Channel8Box_stateChanged(int arg1);

    void on_pauseBtn_clicked();
    void ReadData();
    void bufferData();
    void selectionChanged();
    void slot_SendCommand();
    void slot_checkPort();

private:
    Ui::MainWindow *ui;
    void SimpleDemo();
    void SavwTestResultToCSVFile();
    QTimer *Timer_CHECK;     //定时查询
    QTimer *Timer_UPDATE;   //定时刷新图表和保存数据
    QTimer *Timer_REFLASH;   //定时刷新
    QTimer *Timer_CP = new QTimer(this);        //定时检查串口
    SerialPortSetting *SPSetting;
    QSerialPort *mySerialPort;
    QSet<QString> portSet;
    QVector<int> iVec;
    bool portIsOpen = false;
    QVector<double> PresCh1,PresCh2,PresCh3,PresCh4,PresCh5,PresCh6,PresCh7,PresCh8;
    QVector<double> xCount;
    QTimer *m_timer = new QTimer();
    QByteArray m_buffer;
    QStringList tempList;
};

#endif // MAINWINDOW_H
