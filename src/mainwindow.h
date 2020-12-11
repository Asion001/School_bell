#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QtMultimedia>
#include <QTime>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    void time();
    int setings();
    void ring();
    QTime lessons_list[20];
    QTime end_lessons_list[20];
    QTime ring_now;
    QMediaPlaylist playlist;
    QMediaPlayer player;
    int count_lessons = 0;
    int count_break = 0;
    int lesson_time = 45;
    int volume_player = 50;
    int music_lessons[20] = {4,5};
};
#endif // MAINWINDOW_H
