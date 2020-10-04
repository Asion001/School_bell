#include "alarm.h"
#include "ui_alarm.h"
#include <QTime>
#include <QTimer>
#include <QtMultimedia/QSound>
#include <QDebug>
#include <QFile>

Alarm::Alarm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Alarm)
{
    ui->setupUi(this);
    Curent_Time_Update();
    QTimer *t = new QTimer(this);
    t->setInterval(1000);
    connect(t, &QTimer::timeout, [&]() { Curent_Time_Update(); } );
    t->start();

    save_settings();

    qDebug() << "Start App";

}

Alarm::~Alarm()
{
    delete ui;
}

QString alarm_flag;
void Alarm::Curent_Time_Update(){
    QString time1 = QTime::currentTime().toString();
    ui->label->setText(time1);
    time1.resize(time1.size() - 3);

    QString lesson_time = "17:58";

    if(time1 == lesson_time and time1.compare(alarm_flag) == 1)
    {
      alarm_flag =time1;
      alarm();
    }

}

void Alarm::alarm(){
QSound::play("alarm.wav");
}

void Alarm::on_pushButton_clicked()
{
    alarm();
}

std::string data_txt;
void Alarm::save_settings(){

    settings_read("data.txt");
    qDebug() << "Read data - " << QString::fromStdString(data_txt);
    if(data_txt == "")
    {
        data_txt = "Alarms\n";
        setup();
    }
    //if(data_txt == "") setup();

    settings_write("data.txt", data_txt);
    qDebug() << "Write data - " << QString::fromStdString(data_txt);
}



#include <fstream>
#include <iostream>
#include <string.h>

void Alarm::settings_read(std::string filename)
{
    using namespace std;

    // open a file in read mode.
    std::ifstream infile;
    infile.open(filename);

    infile >> data_txt;
    // close the opened file.
    infile.close();

}

 void Alarm::settings_write(std::string filename, std::string data)
{
    using namespace std;

    // open a file in write mode.
    ofstream outfile;
    outfile.open(filename);

    // write inputted data into the file.
    outfile << data << "\n";

    // close the opened file.
    outfile.close();

}

QStringList lessons;
void Alarm::setup()
{
qDebug() << "Setup";
}

void Alarm::on_pushButton_2_clicked()
{
    setup();
}
