#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Dont warning", "Ring?",
                                    QMessageBox::No|QMessageBox::Yes);
    if (reply == QMessageBox::Yes) ring();
}

void MainWindow::ring()
{
    QSound::play(QCoreApplication::applicationDirPath() + "/bell.wav");
    ring_now = QTime::currentTime();
    qDebug() << "Ring on " << ring_now.toString() << "\n";
}

void MainWindow::time()
{

}

void MainWindow::setings()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {
        ui->label->setText(QTime::currentTime().toString());
        for (int i = 0; i < count_lessons; i++)
        {
            if (QTime::currentTime().hour() == lessons_list[i].hour() and QTime::currentTime().minute() == lessons_list[i].minute())
            {
                if (QTime::currentTime().hour() != ring_now.hour() or QTime::currentTime().minute() != ring_now.minute()) ring();
                else break;
            }
        }

    } );
    timer->start(1000);

    QFile file(QCoreApplication::applicationDirPath() + "/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Data.txt error " << "\n";
        QMessageBox::critical(this,tr("File error"),tr("Cant read file!") );
        return;
    }

    bool file_ok = 0;
    while (!file.atEnd()) {
        QString line = file.readLine();
        qDebug() << "line " << count_lessons << " = " << line << "\n";

        if (line == "Alarms:\n") file_ok = 1;
        else if (file_ok)
        {
            lessons_list[count_lessons] = QTime::fromString(line,"hh':'mm'\n'");
            count_lessons++;
        }
    }

   if (count_lessons < 2)
   {
       QMessageBox::critical(this,tr("File error"),tr("Not have times in data file or wrong format!") );
       return;
   }
}

void MainWindow::music()
{

}
