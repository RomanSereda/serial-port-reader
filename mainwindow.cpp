#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , console(new Console(this))
{
    ui->setupUi(this);

    console->setEnabled(false);

    QGridLayout *layout = findChild<QGridLayout*>("gridLayout");
    layout->addWidget(console);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startStopButton_clicked()
{
    isStarted = !isStarted ? start() : stop();
}

bool MainWindow::start()
{
    auto serialNum = findChild<QLineEdit*>("lineEditSerialNum")->text();
    auto serialSpeed = findChild<QLineEdit*>("lineEditSerialSpeed")->text();

    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::handleReadyRead);
    connect(serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),this, &MainWindow::handleError);

    serialPort->setPortName(serialNum);
    serialPort->setBaudRate(serialSpeed.toInt());

    if(serialPort->open(QIODevice::ReadOnly))
    {
        console->setEnabled(true);
        console->setLocalEchoEnabled(false);
    }

    auto button = findChild<QPushButton*>("startStopButton");
    button->setText("Stop");

    return true;
}

bool MainWindow::stop()
{
    delete serialPort;
    serialPort = nullptr;

    auto button = findChild<QPushButton*>("startStopButton");
    button->setText("Start");

    console->setEnabled(false);

    return false;
}

void MainWindow::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        QString error = serialPort->errorString();
        QCoreApplication::exit(1);
    }
}

void MainWindow::handleReadyRead()
{
    const QByteArray data = serialPort->readAll();
    console->putData(data);
}

