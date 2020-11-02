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
}

void MainWindow::time()
{

}

void MainWindow::setings()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&] { ui->label->setText(QTime::currentTime().toString()); } );
    timer->start(1000);

    QFile file(QCoreApplication::applicationDirPath() + "/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Data.txt error " << "\n";
        QMessageBox::critical(this,tr("File error"),tr("Cant read file!") );
        return;
    }

    bool file_ok = 0;
    int i = 0;
    while (!file.atEnd()) {
        QString line = file.readLine();
        qDebug() << "line " << i << " = " << line << "\n";

        if (line == "Alarms:\n") file_ok = 1;
        else if (file_ok)
        {
            lessons_list[i] = QTime::fromString(line,"hh':'mm'\n'");
            i += 1;
        }
    }

   if (i < 2) QMessageBox::critical(this,tr("File error"),tr("Not have times in data file or wrong format!") );
}

void MainWindow::music()
{

}
