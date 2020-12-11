#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (setings() != 0)
    {
        ui->label->setText("ERROR");
    }
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

void MainWindow::on_pushButton_4_clicked()
{
    player.playlist()->previous();
}

void MainWindow::ring()
{
    QSound::play(QCoreApplication::applicationDirPath() + "/bell.wav");
    ring_now = QTime::currentTime();
    qInfo() << "Ring on " << ring_now.toString() << "\n";
}

void MainWindow::time()
{
    QString status_text;

    for (int i = 0; i < count_lessons; i++)
    {
        if (QTime::currentTime() < lessons_list[i])
        {
            status_text = QString::number(QTime::currentTime().secsTo(lessons_list[i]) / 60) + " minutes to lesson " + QString::number(i + 1);
            break;
        }
        else status_text = "";
    }

    for (int i = 0; i < count_lessons; i++)
    {
        if (QTime::currentTime() < end_lessons_list[i])
        {
            status_text += ". To end break " + QString::number(QTime::currentTime().secsTo(end_lessons_list[i]) / 60);
            break;
        }
        else status_text = "All Lessons are over :) ";
    }

    ui->statusbar->showMessage(status_text);

    ui->label->setText(QTime::currentTime().toString());
    for (int i = 0; i < count_lessons; i++)
    {
        if (QTime::currentTime().hour() == lessons_list[i].hour() and QTime::currentTime().minute() == lessons_list[i].minute())
        {
            if (QTime::currentTime().hour() != ring_now.hour() or QTime::currentTime().minute() != ring_now.minute())
            {
                player.stop();
                QTimer::singleShot(2000,[&](){ring();});
            }
            else break;
        }
        if (QTime::currentTime().hour() == end_lessons_list[i].hour() and QTime::currentTime().minute() == end_lessons_list[i].minute())
        {
            if (QTime::currentTime().hour() != ring_now.hour() or QTime::currentTime().minute() != ring_now.minute())
            {
                ring();
                for (int i2 = 0; i2 < count_break; i2++ )
                {
                    if (i + 1 == music_lessons[i2])
                    {
                        QTimer::singleShot(10000,[&]()
                        {
                            qDebug() << "Play music " << "\n";
                            player.play();
                            player.playlist()->next();
                        } );
                    }
                }
            }
            else break;
        }
    }
}

int MainWindow::setings()
{
    QFile file(QCoreApplication::applicationDirPath() + "/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Data.txt error " << "\n";
        QMessageBox::critical(this,tr("File error"),tr("Cant read file!") );
        return 1;
    }

    int data_index = 0;
    while (!file.atEnd()) {
        QString line = file.readLine();
        qDebug() << "line " << count_lessons << " = " << line << "\n";

        if (line == "\n" or line == "");
        else if (line == "Time:\n") data_index = 1;
        else if (line == "Days:\n") data_index = 2;
        else if (line == "Lessons time:\n") data_index = 3;
        else if (line == "Volume:\n") data_index = 4;
        else if (line == "Music on:\n") data_index = 5;
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
            //line.resize(2);
            lesson_time = line.toInt();
        }
        else if (data_index == 4)
        {
            //line.resize(2);
            volume_player = line.toInt();
        }
        else if (data_index == 5)
        {
            line.resize(1);
            music_lessons[count_break] = line.toInt();
            count_break++;
        }
    }
    if (count_lessons < 1)
    {
        qWarning() << "Not have times in data file or wrong format!\n";
        QMessageBox::critical(this,tr("File error"),tr("Not have times in data file or wrong format!") );
        return 3;
    }

    if (QFile::exists(QCoreApplication::applicationDirPath() + "/bell.wav") != true)
    {
        {
            qWarning() << "Song file not exist\n";
            QMessageBox::critical(this,tr("File error"),tr("Song file not exist!") );
            return 4;
        }
    }

    int pos = 0;
    while (pos < count_lessons)
    {
        if (pos == 0 or lessons_list[pos] >= lessons_list[pos - 1]) pos++;
        else
        {
            qSwap(lessons_list[pos], lessons_list[pos - 1]);
            pos -= 1;
        }
    }

    for (int i = 0;i < count_lessons;i++ ) qDebug() << lessons_list[i].toString() << " Sorted lessons" << i;
    if (lesson_time !=0)
    {
        for (int i = 0; i < count_lessons;i++ )
        {
            end_lessons_list[i] = lessons_list[i].addSecs(lesson_time * 60);
            qDebug() << end_lessons_list[i].toString() << " end lesson time lis \n";
        }
    }
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]() {time();} );
    timer->start(1000);

    QStringList music_paths = QDir(QCoreApplication::applicationDirPath() + "/media").entryList(QDir::Files);

    if (music_paths.length() == 0)
    {
        qWarning() << "Song file not exist\n";
        QMessageBox::critical(this,tr("File error"),tr("Media folder is empty!") );
        //return 1;
    }else
    {
        for (int i = 0; i < music_paths.length(); i++) music_paths[i] = QCoreApplication::applicationDirPath() + "/media/" + music_paths[i];
        for (int i = 0; i < music_paths.length(); i++) playlist.addMedia(QMediaContent(QUrl::fromLocalFile(music_paths[i])));
        playlist.shuffle();
        playlist.setPlaybackMode(QMediaPlaylist::Loop);
        playlist.setCurrentIndex(0);
        player.setPlaylist(&playlist);
        player.setVolume(volume_player);
    }

    qDebug() << "Files:\n" << music_paths << "\n" ;
    qDebug() << "Day of week now " << QDate::currentDate().dayOfWeek() << "\n";
    qInfo() << playlist.mediaCount() << " songs\n";
    qInfo() << "Lesson time " << lesson_time << "\n";
    qInfo() << "Lesson count " << count_lessons << "\n";

    return 0;
}
