#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startStopButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort* serialPort;
    bool isStarted = false;

    bool start();
    bool stop();

    void handleError(QSerialPort::SerialPortError serialPortError);
    void handleReadyRead();
};
#endif // MAINWINDOW_H
