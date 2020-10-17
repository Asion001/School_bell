#include "alarm.h"
#include "ui_alarm.h"
#include <QTime>
#include <QTimer>
#include <QtMultimedia/QSound>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <string>

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


std::string lessons[50];
QString alarm_flag;
int alarm_number = 0;
std::string data_txt[50];
int data_size = 0;

void Alarm::Curent_Time_Update()
{
    QString time1 = QTime::currentTime().toString();
    ui->label->setText(time1);
    time1.resize(time1.size() - 3);

    int time_to = QTime::currentTime().secsTo(QTime::fromString(QString::fromStdString(lessons[alarm_number]),"hh':'mm" )) / 60;
    QString time_to_out;

    if(data_size > alarm_number and alarm_number > 0)
    {
    if(time_to < 60) time_to_out = QString::number(time_to) + " minutes to next ring ";
    else time_to_out = QString::number(time_to / 60) + " hours to ring";

    //qDebug() << alarm_number << "  " << data_size << " " << time_to_out;

    ui->statusbar->showMessage(time_to_out);

    } else ui->statusbar->showMessage("");


    if(time1 != alarm_flag)
    {
      alarm_flag = time1;
      for (int i = 0; i < 50; i++)
      {
          if(time1 == QString::fromStdString(lessons[i]))
          {
              alarm_number = i+1;
              qDebug() << alarm_number << "  " << data_size << "  " << i << " " << time_to_out;
              alarm();
              break;
          }
       }

    }
}



void Alarm::alarm()
{
QString alarm_path = QCoreApplication::applicationDirPath() + "/alarm.wav";
QSound::play(alarm_path);
qDebug() << "Play on " << QTime::currentTime().toString() << "\n";
}



void Alarm::on_pushButton_clicked()
{
    alarm();
}



void Alarm::save_settings()
{
    QString data_path = QCoreApplication::applicationDirPath() + "/data.txt";

    using namespace std;
    qDebug() << data_path;
    settings_read(data_path.toStdString() , data_txt);

    for (int i = 1; i < 50; i++)
    {
        if(data_txt[i] != "")
        {
            qDebug() << QString::fromStdString(data_txt[i]) << "\n";
            data_size++;
            lessons[i-1] = data_txt[i];
        }
    }

    if(data_txt[0] != "Alarms:" or data_size < 2)
    {
        qDebug() << "Wrong data file\n size - " << data_size;
        QMessageBox::critical(this,tr("ERROR"),tr("Wrong data file!") );
    }

}



void Alarm::settings_read(std::string filename, std::string data[50])
{
    using namespace std;

    string tmp_data;
    ifstream IN(filename);

    int i = 0;
    while (getline (IN, tmp_data))
    {
        data[i] = tmp_data;
        i++;
    }

    IN.close();
}



void Alarm::settings_write(std::string filename, std::string data[50])
{
    using namespace std;
    ofstream IN(filename);

    for (int i = 0; i < 50; i++)
    {
        if(data[i] != "") IN << data[i] << "\n";
    }

    IN.close();
}
