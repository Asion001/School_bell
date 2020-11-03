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

private:
    Ui::MainWindow *ui;
    void time();
    void setings();
    void ring();
    QTime lessons_list[100];
    QTime ring_now;
    QMediaPlaylist playlist;
    QMediaPlayer player;
    int count_lessons = 0;
};
#endif // MAINWINDOW_H
