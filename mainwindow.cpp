#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QStringList>
#include <QDebug>
#include <iostream>
#include <string>
#include <QChar>
#include <QButtonGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Serial Port Com");
    getComPorts();

    QButtonGroup settingsGroup1,settingsGroup2,settingsGroup3;
    /*settingsGroup1 = new QButtonGroup;
    settingsGroup2 = new QButtonGroup;
    settingsGroup3 = new QButtonGroup;*/

    settingsGroup1.addButton(ui->displayOnOffButton);
    settingsGroup2.addButton(ui->cursorOnButton);
    settingsGroup3.addButton(ui->blinkCursorButton);

    serial = new SerialPort;
    connect(serial,SIGNAL(readyRead()),this,SLOT(readData()));
    setDefaultSerialParameters();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_send_button_clicked()
{
    QString temp;
    temp = ui->message_line->text();
    QByteArray convertedTemp;
    convertedTemp = temp.toLocal8Bit().constData();
    serial->sendMessage1602(convertedTemp);
    ui->message_line->clear();
}

void MainWindow::readData()
{
    ui->chat_box->insertPlainText(serial->readAll());
}

void MainWindow::on_comPortButton_clicked()
{
    getComPorts();
}

void MainWindow::getComPorts()
{
    ui->comPortBox->clear();
    list = QSerialPortInfo::availablePorts();

    if(list.isEmpty())
        ui->comPortBox->addItem("No Port Detected");

    QList<QString> portList;

    for(int i=0; i<list.length(); i++)
    {
        portList.push_back(list[i].portName());
    }
    ui->comPortBox->addItems(portList);
}

void MainWindow::on_connectButton_clicked()
{ 
    int index = -1;
    for(int i=0; i<list.length(); i++)
    {
        if(list[i].portName() == ui->comPortBox->currentText())
        {
            index = i;
            break;
        }
    }
    if(index != -1)
        serial->setPort(list[index]);
    else
        ui->chat_box->appendPlainText("Error: Selected port cannot be found! Please refresh port list");

    int portOpen = serial->open(QIODeviceBase::ReadWrite);
    if(portOpen)
        ui->chat_box->appendPlainText("Selected port is now open");
    else
        ui->chat_box->appendPlainText("Error: cannot open selected port");
}

void MainWindow::on_baudRateBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            serial->setBaudRate(SerialPort::BaudRate::Baud1200);
            break;
        case 1:
            serial->setBaudRate(SerialPort::BaudRate::Baud2400);
            break;
        case 2:
            serial->setBaudRate(SerialPort::BaudRate::Baud4800);
            break;
        case 3:
            serial->setBaudRate(SerialPort::BaudRate::Baud9600);
            break;
        case 4:
            serial->setBaudRate(SerialPort::BaudRate::Baud19200);
            break;
        case 5:
            serial->setBaudRate(SerialPort::BaudRate::Baud38400);
            break;
        case 6:
            serial->setBaudRate(SerialPort::BaudRate::Baud57600);
            break;
        case 7:
            serial->setBaudRate(SerialPort::BaudRate::Baud115200);
            break;
    }
}

void MainWindow::on_dataBitBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            serial->setDataBits(SerialPort::DataBits::Data5);
            break;
        case 1:
            serial->setDataBits(SerialPort::DataBits::Data6);
            break;
        case 2:
            serial->setDataBits(SerialPort::DataBits::Data7);
            break;
        case 3:
            serial->setDataBits(SerialPort::DataBits::Data8);
            break;
    }
}

void MainWindow::on_stopBitBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            serial->setStopBits(SerialPort::StopBits::OneStop);
            break;
        case 1:
            serial->setStopBits(SerialPort::StopBits::OneAndHalfStop);
            break;
        case 2:
            serial->setStopBits(SerialPort::StopBits::TwoStop);
            break;
    }
}

void MainWindow::on_parityBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            serial->setParity(SerialPort::Parity::NoParity);
            break;
        case 1:
            serial->setParity(SerialPort::Parity::EvenParity);
            break;
        case 2:
            serial->setParity(SerialPort::Parity::OddParity);
            break;
        case 3:
            serial->setParity(SerialPort::Parity::SpaceParity);
            break;
        case 4:
            serial->setParity(SerialPort::Parity::MarkParity);
            break;
    }
}

void MainWindow::on_flowControlBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            serial->setFlowControl(SerialPort::FlowControl::NoFlowControl);
            break;
        case 1:
            serial->setFlowControl(SerialPort::FlowControl::HardwareControl);
            break;
        case 2:
            serial->setFlowControl(SerialPort::FlowControl::SoftwareControl);
            break;
    }
}

void MainWindow::setDefaultSerialParameters()
{
    ui->baudRateBox->addItems(serial->baudRateList);
    ui->dataBitBox->addItems(serial->dataBitList);
    ui->stopBitBox->addItems(serial->stopBitList);
    ui->parityBox->addItems(serial->parityList);
    ui->flowControlBox->addItems(serial->flowControlList);

    ui->baudRateBox->setCurrentIndex(3);
    ui->dataBitBox->setCurrentIndex(3);
    ui->stopBitBox->setCurrentIndex(0);
    ui->parityBox->setCurrentIndex(0);
    ui->flowControlBox->setCurrentIndex(0);
}

void MainWindow::on_clearButton_clicked()
{
    serial->sendCommand1602(clear1602);
}

void MainWindow::on_secondLineButton_clicked()
{
    serial->sendCommand1602(secondLine1602);
}

void MainWindow::on_moveCursorButton_clicked()
{
    serial->sendCommand1602(moveCursor1602);
}

void MainWindow::apply1602Settings(void)
{
    serial->command.clear();
    serial->command.append(options1602);
    if(ui->displayOnOffButton->isChecked())
        serial->command.append('1');
    else
        serial->command.append('0');
    if(ui->cursorOnButton->isChecked())
        serial->command.append('1');
    else
        serial->command.append('0');
    if(ui->blinkCursorButton->isChecked())
        serial->command.append('1');
    else
        serial->command.append('0');
    serial->command.append(terminator1602);
    serial->write(serial->command);
}

void MainWindow::on_displayOnOffButton_clicked()
{
    MainWindow::apply1602Settings();
}

void MainWindow::on_cursorOnButton_clicked()
{
    MainWindow::apply1602Settings();
}

void MainWindow::on_blinkCursorButton_clicked()
{
    MainWindow::apply1602Settings();
}

