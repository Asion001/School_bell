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
    reply = QMessageBox::question(this, "Are you sure?", "Ring?",
                                    QMessageBox::No|QMessageBox::Yes);
    if (reply == QMessageBox::Yes) ring();
}

void MainWindow::on_pushButton_2_clicked()
{
    qDebug() << player.state() << "\n";
    if (player.state() == QMediaPlayer::PausedState or player.state() == QMediaPlayer::StoppedState)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Are you sure?", "Play music?",
                                        QMessageBox::No|QMessageBox::Yes);
        if (reply == QMessageBox::Yes) player.play();
    }
    else player.pause();
}

void MainWindow::on_pushButton_3_clicked()
{
    player.playlist()->next();
}

void MainWindow::ring()
{
    QSound::play(QCoreApplication::applicationDirPath() + "/bell.wav");
    ring_now = QTime::currentTime();
    qInfo() << "Ring on " << ring_now.toString() << "\n";
}

void MainWindow::time()
{
    ui->label->setText(QTime::currentTime().toString());
    for (int i = 0; i < count_lessons; i++)
    {
        if (QTime::currentTime().hour() == lessons_list[i].hour() and QTime::currentTime().minute() == lessons_list[i].minute())
        {
            if (QTime::currentTime().hour() != ring_now.hour() or QTime::currentTime().minute() != ring_now.minute())
            {
                player.stop();
                ring();
            }
            else break;
        }
        if (QTime::currentTime().hour() == end_lessons_list[i].hour() and QTime::currentTime().minute() == end_lessons_list[i].minute())
        {
            if (QTime::currentTime().hour() != ring_now.hour() or QTime::currentTime().minute() != ring_now.minute())
            {
                ring();
                QTimer::singleShot(10000,[&]()
                {
                    qDebug() << "Play music " << "\n";
                    player.play();
                    player.playlist()->next();
                } );

            }
            else break;
        }
    }
}

void MainWindow::setings()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {time();} );
    timer->start(1000);

    QFile file(QCoreApplication::applicationDirPath() + "/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Data.txt error " << "\n";
        QMessageBox::critical(this,tr("File error"),tr("Cant read file!") );
        return;
    }

    int data_index = 0;
    while (!file.atEnd()) {
        QString line = file.readLine();
        qDebug() << "line " << count_lessons << " = " << line << "\n";

        if (line == "Time:\n") data_index = 1;
        else if (line == "Days:\n") data_index = 2;
        else if (line == "Lesson time:\n") data_index = 3;
        else if (data_index == 1)
        {
            lessons_list[count_lessons] = QTime::fromString(line,"hh':'mm'\n'");
            count_lessons++;
        }
        else if (data_index == 2)
        {
            //here must be days
        }
        else if (data_index == 3)
        {
            line.resize(2);
            lesson_time = line.toInt();
        }

    }
    if (count_lessons < 2)
    {
        qWarning() << "Not have times in data file or wrong format!\n";
        QMessageBox::critical(this,tr("File error"),tr("Not have times in data file or wrong format!") );
        return;
    }

    for (int i = 0;i < count_lessons;i++ )
        {
            end_lessons_list[i] = lessons_list[i].addSecs(lesson_time * 60);
            qDebug() << end_lessons_list[i].toString() << " end lesson time lis \n";
        }

    QStringList music_paths = QDir(QCoreApplication::applicationDirPath() + "/media").entryList(QDir::Files);
    for (int i = 0; i < music_paths.length(); i++) music_paths[i] = QCoreApplication::applicationDirPath() + "/media/" + music_paths[i];
    for (int i = 0; i < music_paths.length(); i++) playlist.addMedia(QMediaContent(QUrl::fromLocalFile(music_paths[i])));
    qDebug() << "Files:\n" << music_paths << "\n" ;
    qDebug() << "Day of week now " << QDate::currentDate().dayOfWeek() << "\n";
    qInfo() << playlist.mediaCount() << " songs\n";
    qInfo() << "Lesson time " << lesson_time << "\n";
    qInfo() << "Lesson count " << count_lessons << "\n";
    playlist.shuffle();
    playlist.setCurrentIndex(0);
    player.setPlaylist(&playlist);
    player.setVolume(50);
}
