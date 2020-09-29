#include "alarm.h"
#include "ui_alarm.h"
#include <QTime>
#include <QTimer>
#include <QtMultimedia/QSound>
#include <QDebug>

Alarm::Alarm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Alarm)
{
    ui->setupUi(this);

    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t, &QTimer::timeout, [&]() { Curent_Time_Update(); } );
    t->start();

}

Alarm::~Alarm()
{
    delete ui;
}


void Alarm::on_pushButton_pressed()
{
    ui->label->setText(QTime::currentTime().toString());
}

void Alarm::Curent_Time_Update(){
    QString time1 = QTime::currentTime().toString();
    ui->label->setText(time1);

}

void Alarm::alarm(){
QSound::play("./alarm.wav");
}
