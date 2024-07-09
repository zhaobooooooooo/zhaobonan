#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isWorking(true)
{
    ui->setupUi(this);

    // 初始化时间显示
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    timer->start(1000);

    workStartTime = QTime::currentTime();
    restStartTime = QTime::currentTime().addSecs(7200);

    // 连接SOS按钮
    connect(ui->sosButton, &QPushButton::clicked, this, &MainWindow::sendSOS);

    updateLabels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTime()
{
    QTime currentTime = QTime::currentTime();
    ui->timeLabel->setText(currentTime.toString("hh:mm:ss"));
    checkWorkRest();
}

void MainWindow::checkWorkRest()
{
    QTime currentTime = QTime::currentTime();
    int elapsedWorkTime = workStartTime.secsTo(currentTime);
    int elapsedRestTime = restStartTime.secsTo(currentTime);

    if (isWorking)
    {
        if (elapsedWorkTime >= 7200)
        {
            ui->alertLabel->setText("ALERT: Need to rest!");
            isWorking = false;
            restStartTime = currentTime;
        }
        else
        {
            int remainSeconds = 7200 - elapsedWorkTime;
            ui->workRemainLabel->setText(QString(" %1:%2:%3")
                                             .arg(remainSeconds / 3600, 2, 10, QChar('0'))
                                             .arg((remainSeconds % 3600) / 60, 2, 10, QChar('0'))
                                             .arg(remainSeconds % 60, 2, 10, QChar('0')));
        }
    }
    else
    {
        if (elapsedRestTime >= 3600)
        {
            ui->alertLabel->setText("");
            isWorking = true;
            workStartTime = currentTime;
        }
        else
        {
            ui->currentRestLabel->setText(QString(" %1:%2:%3")
                                              .arg(elapsedRestTime / 3600, 2, 10, QChar('0'))
                                              .arg((elapsedRestTime % 3600) / 60, 2, 10, QChar('0'))
                                              .arg(elapsedRestTime % 60, 2, 10, QChar('0')));
        }
    }

    ui->nextRestTimeLabel->setText(restStartTime.addSecs(3600).toString("hh:mm"));
    ui->totalWorkLabel->setText(QString("%1:%2:%3")
                                    .arg(elapsedWorkTime / 3600, 2, 10, QChar('0'))
                                    .arg((elapsedWorkTime % 3600) / 60, 2, 10, QChar('0'))
                                    .arg(elapsedWorkTime % 60, 2, 10, QChar('0')));
    ui->totalRestLabel->setText(QString("%1:%2:%3")
                                    .arg(elapsedRestTime / 3600, 2, 10, QChar('0'))
                                    .arg((elapsedRestTime % 3600) / 60, 2, 10, QChar('0'))
                                    .arg(elapsedRestTime % 60, 2, 10, QChar('0')));
    ui->lastRestTimeLabel->setText(workStartTime.addSecs(elapsedWorkTime).toString("hh:mm"));
}

void MainWindow::sendSOS()
{
    QMessageBox::warning(this, "SOS", "SOS signal sent!");
}

void MainWindow::updateLabels()
{
    ui->workRemainLabel->setText(" 2:00:00");
    ui->currentRestLabel->setText(" 00:00:00");
    ui->nextRestTimeLabel->setText( restStartTime.toString("hh:mm"));
    ui->totalWorkLabel->setText("00:00:00");
    ui->totalRestLabel->setText(" 00:00:00");
    ui->lastRestTimeLabel->setText(" --:--");
    ui->alertLabel->setText("");
}
