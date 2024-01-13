#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("PressureRealTimeDisplay"));
    //添加串口代码 td
    //查找可用的串口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortNumcomboBox->addItem(serial.portName());
            serial.close();
        }
    }
    ui->BaudRatecomboBox->setCurrentIndex(0);
    SimpleDemo();
    Timer_CHECK = new QTimer(this);
    connect(Timer_CHECK,SIGNAL(timeout()),this,SLOT(slot_SendCommand()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_OpenBtn_clicked()
{
    if(ui->OpenBtn->text() == tr("Open"))
    {
        mySerialPort = new QSerialPort(this);
        mySerialPort->setPortName(ui->PortNumcomboBox->currentText());
        mySerialPort->open(QIODevice::ReadWrite);
        mySerialPort->setDataBits(QSerialPort::Data8);
        switch(ui->BaudRatecomboBox->currentIndex())
        {
        case 0:
            mySerialPort->setBaudRate(QSerialPort::Baud9600);
            break;
        case 1:
            mySerialPort->setBaudRate(QSerialPort::Baud115200);
            break;
        default:
            break;
        }
        mySerialPort->setParity(QSerialPort::NoParity);
        mySerialPort->setStopBits(QSerialPort::OneStop);
        mySerialPort->setFlowControl(QSerialPort::NoFlowControl);
        ui->PortNumcomboBox->setEnabled(false);
        ui->BaudRatecomboBox->setEnabled(false);
        ui->OpenBtn->setText("Close");
        connect(mySerialPort,SIGNAL(readyRead()),this,SLOT(bufferData()));
        connect(m_timer,SIGNAL(timeout()),this,SLOT(ReadData()));
        connect(Timer_CP,SIGNAL(timeout()),this,SLOT(slot_checkPort()));
        Timer_CP->start(1000);
        ui->StartButton->setEnabled(true);
        ui->pauseBtn->setEnabled(false);
    }
    else
    {
        mySerialPort->clear();
        mySerialPort->close();
        mySerialPort->deleteLater();
        if(Timer_CP->isActive())
        {
            Timer_CP->stop();
        }
        Timer_CP->disconnect();
        if(Timer_CHECK->isActive())
        {
            Timer_CHECK->stop();
        }
        ui->PortNumcomboBox->setEnabled(true);
        ui->BaudRatecomboBox->setEnabled(true);
        ui->OpenBtn->setText("Open");
        ui->StartButton->setEnabled(false);
        ui->pauseBtn->setEnabled(false);
    }
}
void MainWindow::slot_checkPort()
{
    QSet<QString> portSet;
    foreach(const QSerialPortInfo &Info, QSerialPortInfo::availablePorts())
    {
        portSet.insert(Info.portName());
    }
    if(portSet.find(mySerialPort->portName()) == portSet.end())
    {
        QMessageBox::warning(this,\
                             QString::fromLocal8Bit("Application error"),\
                             QString::fromLocal8Bit("Fail with the following error: \nUsart failed\n\nPort:%1")\
                             .arg(mySerialPort->portName()),\
                             QMessageBox::Close);
        emit on_OpenBtn_clicked();
    }
}

void MainWindow::bufferData()
{
    m_timer->start(50);
    m_buffer.append(mySerialPort->readAll());
}

void MainWindow::ReadData()
{
    QString tempData;
    m_timer->stop();
    //把m_buffer拿去解析
    tempData = m_buffer;
    tempData = tempData.replace("PRESSURE1= ","").replace("PRESSURE2= ","").replace("PRESSURE3= ","")\
            .replace("PRESSURE4= ","").replace("PRESSURE5= ","").replace("PRESSURE6= ","")\
            .replace("PRESSURE7= ","").replace("PRESSURE8= ","");
    tempData = tempData.simplified();
    tempList = tempData.split(',');

    PresCh1.append(tempList[0].toDouble());
    PresCh2.append(tempList[1].toDouble());
    PresCh3.append(tempList[2].toDouble());
    PresCh4.append(tempList[3].toDouble());
    PresCh5.append(tempList[4].toDouble());
    PresCh6.append(tempList[5].toDouble());
    PresCh7.append(tempList[6].toDouble());
    PresCh8.append(tempList[7].toDouble());
    xCount.append(xCount.count());
    UpdateChartAndSaveData();
    SavwTestResultToCSVFile();

    m_buffer.clear();
}
void MainWindow::on_StartButton_clicked()
{
    Timer_CHECK->start(500);
    ui->StartButton->setEnabled(false);
    ui->pauseBtn->setEnabled(true);
}
void MainWindow::on_pauseBtn_clicked()
{
    if(Timer_CHECK->isActive())
    {
        Timer_CHECK->stop();            //停止定时器
    }
    Timer_CHECK->disconnect();
    ui->pauseBtn->setEnabled(false);
    ui->StartButton->setEnabled(true);
}
void MainWindow::UpdateChartAndSaveData()
{
     /**自动调整XY坐标范围**/
    ui->myCustomPlot->graph(0)->rescaleAxes();
    ui->myCustomPlot->graph(1)->rescaleAxes();
    ui->myCustomPlot->graph(2)->rescaleAxes();
    ui->myCustomPlot->graph(3)->rescaleAxes();
    ui->myCustomPlot->graph(4)->rescaleAxes();
    ui->myCustomPlot->graph(5)->rescaleAxes();
    ui->myCustomPlot->graph(6)->rescaleAxes();
    ui->myCustomPlot->graph(7)->rescaleAxes();

    ui->myCustomPlot->graph(0)->setData(xCount,PresCh1);
    ui->myCustomPlot->graph(1)->setData(xCount,PresCh2);
    ui->myCustomPlot->graph(2)->setData(xCount,PresCh3);
    ui->myCustomPlot->graph(3)->setData(xCount,PresCh4);
    ui->myCustomPlot->graph(4)->setData(xCount,PresCh5);
    ui->myCustomPlot->graph(5)->setData(xCount,PresCh6);
    ui->myCustomPlot->graph(6)->setData(xCount,PresCh7);
    ui->myCustomPlot->graph(7)->setData(xCount,PresCh8);
    ui->myCustomPlot->replot();
}

void MainWindow::SavwTestResultToCSVFile()
{
    QString csvFileName = QApplication::applicationDirPath() + "/TestData" + \
                          QDateTime::currentDateTime().toString("yyyyMMdd") + ".csv";
    QFile file(csvFileName);
    if(!file.exists())    //文件不存在的时候便新建，以当天日期为命名
    {
        file.open(QIODevice::WriteOnly);
        QTextStream txtOutPut(&file);
        txtOutPut << "Unit(kPa)\n";
        txtOutPut << "Time,Channel1,Channel2,Channel3,Channel4,Channel5,Channel6,Channel7,Channel8\n";
        file.close();
    }
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    QTextStream txtOutPut(&file);

    QString msg = QDateTime::currentDateTime().toString("hh:mm:ss")+","\
            + tempList[0] + "," \
            + tempList[1] + "," \
            + tempList[2] + "," \
            + tempList[3] + "," \
            + tempList[4] + "," \
            + tempList[5] + "," \
            + tempList[6] + "," \
            + tempList[7] + "\n" ;               //这里只是模拟数据，具体数据做相对应改动即可
    txtOutPut << msg;
    file.flush();
    file.close();
}

void MainWindow::SimpleDemo()
{
    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(0)->setPen(QPen(Qt::darkRed));
   // ui->myCustomPlot->graph(0)->setData(x,y);
    ui->myCustomPlot->graph(0)->setVisible(false);

    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(1)->setPen(QPen(Qt::darkGreen));
    //ui->myCustomPlot->graph(1)->setData(XChannel1,temp1);
    ui->myCustomPlot->graph(1)->setVisible(false);

    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(2)->setPen(QPen(Qt::blue));
   // ui->myCustomPlot->graph(2)->setData(XChannel2,temp1);
    ui->myCustomPlot->graph(2)->setVisible(false);

    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(3)->setPen(QPen(Qt::black));
   // ui->myCustomPlot->graph(3)->setData(XChannel3,temp1);
    ui->myCustomPlot->graph(3)->setVisible(false);

    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(4)->setPen(QPen(Qt::darkCyan));
    //ui->myCustomPlot->graph(4)->setData(XChannel4,temp1);
    ui->myCustomPlot->graph(4)->setVisible(false);

    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(5)->setPen(QPen(Qt::darkMagenta));
    //ui->myCustomPlot->graph(5)->setData(XChannel5,temp1);
    ui->myCustomPlot->graph(5)->setVisible(false);

    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(6)->setPen(QPen(Qt::red));
    //ui->myCustomPlot->graph(6)->setData(XChannel6,temp1);
    ui->myCustomPlot->graph(6)->setVisible(false);

    ui->myCustomPlot->addGraph();
    ui->myCustomPlot->graph(7)->setPen(QPen(Qt::darkGray));
    //ui->myCustomPlot->graph(7)->setData(XChannel7,temp1);
    ui->myCustomPlot->graph(7)->setVisible(false);

    ui->myCustomPlot->xAxis->setLabel("time");
    ui->myCustomPlot->yAxis->setLabel("Pressure(kPa)");

    ui->myCustomPlot->legend->setVisible(true);
    QString GraphName[8] = {"Channel1","Channel2","Channel3","Channel4","Channel5","Channel6","Channel7","Channel8"};
    for(int i=0;i<8;i++)
    {
        ui->myCustomPlot->graph(i)->setName(GraphName[i]);
    }
    /**自动调整XY坐标范围**/
    connect(ui->myCustomPlot->xAxis,SIGNAL(rangeChanged(QCPRange)),ui->myCustomPlot->xAxis2,SLOT(setRange(QCPRange)));
    connect(ui->myCustomPlot->yAxis,SIGNAL(rangeChanged(QCPRange)),ui->myCustomPlot->yAxis2,SLOT(setRange(QCPRange)));

    ui->myCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables|QCP::iSelectAxes|QCP::iSelectLegend);
    ui->myCustomPlot->legend->setSelectableParts(QCPLegend::spItems);
    connect(ui->myCustomPlot,SIGNAL(selectionChangedByUser()),this,SLOT(selectionChanged()));
}
void MainWindow::selectionChanged()
{
    if(ui->myCustomPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis)||ui->myCustomPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels)||
            ui->myCustomPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis)||ui->myCustomPlot->xAxis2
            ->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->myCustomPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->myCustomPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    if(ui->myCustomPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis)||ui->myCustomPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels)||
            ui->myCustomPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis)||ui->myCustomPlot->yAxis2
            ->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
        ui->myCustomPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
        ui->myCustomPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
    for(int i=0;i<ui->myCustomPlot->graphCount();++i)
    {
        QCPGraph *graph = ui->myCustomPlot->graph(i);
        QCPPlottableLegendItem *item = ui->myCustomPlot->legend->itemWithPlottable(graph);
        if(item->selected()||graph->selected())
        {
            item->setSelected(true);
            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
        }
    }
}
void MainWindow::slot_SendCommand()
{
    QString sendCommand("GETPRESSURE:15\r\n");
    mySerialPort->write(sendCommand.toUtf8().data());
}
void MainWindow::on_Channel1Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(0)->setVisible(true);
        ui->Channel1Box->setStyleSheet("QCheckBox{color:darkRed}");
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(0)->setVisible(false);
        ui->Channel1Box->setStyleSheet("QCheckBox{color:black}");
    }
    ui->myCustomPlot->replot();
}

void MainWindow::on_Channel2Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(1)->setVisible(true);
        ui->Channel2Box->setStyleSheet("QCheckBox{color:darkGreen}");
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(1)->setVisible(false);
        ui->Channel2Box->setStyleSheet("QCheckBox{color:black}");
    }
    ui->myCustomPlot->replot();
}

void MainWindow::on_Channel3Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(2)->setVisible(true);
        ui->Channel3Box->setStyleSheet("QCheckBox{color:blue}");
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(2)->setVisible(false);
        ui->Channel3Box->setStyleSheet("QCheckBox{color:black}");
    }
    ui->myCustomPlot->replot();
}

void MainWindow::on_Channel4Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(3)->setVisible(true);
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(3)->setVisible(false);
    }
    ui->myCustomPlot->replot();
}

void MainWindow::on_Channel5Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(4)->setVisible(true);
        ui->Channel5Box->setStyleSheet("QCheckBox{color:darkCyan}");
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(4)->setVisible(false);
        ui->Channel5Box->setStyleSheet("QCheckBox{color:black}");
    }
    ui->myCustomPlot->replot();
}

void MainWindow::on_Channel6Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(5)->setVisible(true);
        ui->Channel6Box->setStyleSheet("QCheckBox{color:darkMagenta}");
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(5)->setVisible(false);
        ui->Channel6Box->setStyleSheet("QCheckBox{color:black}");
    }
    ui->myCustomPlot->replot();
}

void MainWindow::on_Channel7Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(6)->setVisible(true);
        ui->Channel7Box->setStyleSheet("QCheckBox{color:red}");
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(6)->setVisible(false);
        ui->Channel7Box->setStyleSheet("QCheckBox{color:black}");
    }
    ui->myCustomPlot->replot();
}

void MainWindow::on_Channel8Box_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->myCustomPlot->graph(7)->setVisible(true);
        ui->Channel8Box->setStyleSheet("QCheckBox{color:darkGray}");
    }
    else if(arg1 == 0)
    {
        ui->myCustomPlot->graph(7)->setVisible(false);
        ui->Channel8Box->setStyleSheet("QCheckBox{color:black}");
    }
    ui->myCustomPlot->replot();
}




