#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getComPorts();
    void apply1602Settings(void);

private slots:
    void on_send_button_clicked();
    void readData();
    void on_comPortButton_clicked();
    void on_connectButton_clicked();
    void setDefaultSerialParameters(void);
    void on_baudRateBox_currentIndexChanged(int index);
    void on_dataBitBox_currentIndexChanged(int index);
    void on_stopBitBox_currentIndexChanged(int index);
    void on_parityBox_currentIndexChanged(int index);
    void on_flowControlBox_currentIndexChanged(int index);
    void on_clearButton_clicked();
    void on_secondLineButton_clicked();
    void on_moveCursorButton_clicked();
    void on_displayOnOffButton_clicked();
    void on_cursorOnButton_clicked();
    void on_blinkCursorButton_clicked();

private:
    QList<QSerialPortInfo> list;
    SerialPort* serial;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
