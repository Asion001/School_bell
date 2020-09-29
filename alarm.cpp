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

QString data_txt;
void Alarm::save_settings(){

settings_read("data.txt");
if(data_txt == "") setup();

settings_write("data.txt");

qDebug() << data_txt;
}

void Alarm::settings_read(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open the file for reading";
        return;
    }
    QTextStream in(&file);
    QString myText = in.readAll();
    data_txt = myText;
    file.close();
}

void Alarm::settings_write(QString filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly |
                  QFile::Text))
    {
        qDebug() << " Could not open file for writing";
        return;
    }
    QTextStream out(&file);
    out << data_txt;
    file.flush();
    file.close();
}
QStringList lessons;
void Alarm::setup(){

}


void Alarm::on_pushButton_2_clicked()
{
    setup();
}
