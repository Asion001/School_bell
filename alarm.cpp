#include "alarm.h"
#include "ui_alarm.h"
#include <QTime>
#include <QTimer>
#include <QtMultimedia/QSound>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

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
void Alarm::Curent_Time_Update(){
    QString time1 = QTime::currentTime().toString();
    ui->label->setText(time1);
    time1.resize(time1.size() - 3);

    if(time1 != alarm_flag)
    {
      alarm_flag = time1;
      for (int i = 0; i < 50; i++)
      {
          if(time1 == QString::fromStdString(lessons[i]))
          {
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

std::string data_txt[50];
int data_size = 0;



void Alarm::save_settings()
{
    QString data_path = QCoreApplication::applicationDirPath() + "/data.txt";

    using namespace std;
    qDebug() << data_path;
    settings_read(data_path.toStdString() , data_txt);

    for (int i = 0; i < 50; i++)
    {
        if(data_txt[i] != "")
        {
            qDebug() << QString::fromStdString(data_txt[i]) << "\n";
            data_size++;
        }
    }

    if(data_txt[0] != "Alarms:" or data_size < 2)
    {
        qDebug() << "Wrong data file\n size - " << data_size;
        QMessageBox::critical(this,tr("ERROR"),tr("Wrong data file!") );
    }

    for (int i = 1; i < 50; i++)
    {
        if(data_txt[i] != "")
        {
            lessons[i-1] = data_txt[i];
        }
    }

}

#include<iostream>
#include<fstream>

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


void Alarm::setup()
{
    qDebug() << "Setup";
}

void Alarm::on_pushButton_2_clicked()
{
    setup();
}


