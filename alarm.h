#ifndef ALARM_H
#define ALARM_H

#include <QMainWindow>
#include <string.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Alarm; }
QT_END_NAMESPACE

class Alarm : public QMainWindow
{
    Q_OBJECT

public:
    Alarm(QWidget *parent = nullptr);
    ~Alarm();

private slots:

    void Curent_Time_Update();
    void alarm();
    void on_pushButton_clicked();
    void save_settings();
    void settings_read(std::string filename, std::string data[50]);
    void settings_write(std::string filename, std::string data[50]);
    void setup();
    void on_pushButton_2_clicked();

private:
    Ui::Alarm *ui;
};
#endif // ALARM_H
