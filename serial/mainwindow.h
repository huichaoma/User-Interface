#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QProgressBar>
#include <QAction>
#include <QPushButton>
#include <QMessageBox>
#include "power.h"
#include "about.h"
#include "exploit.h"
#include "nature.h"
#include "serial_config.h"
#include "four_electrodes.h"
#include "resistance.h"
#include "Sendthread.h"
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void powerAction();
    void potentialAction();
    void resistanceAction();
    void four_electrodesAction();
    void Reboot_MCU();
    void Reboot_pi();
    void Shut_pi();
    void aboutAction();
    void network();
    void quit();
    void receiveData(QString data);
    void Receive_Power(int value);
    void Receive_Com(bool sign);
    void Receive_Com_Block(bool sign);

private:
    QProgressBar *pow;
    QLabel *per1;
    QLabel *energy;
    power *Power_Interface;
    nature *Nature_Interface;
    resistance *Resistance_Interface;
    four_electrodes *Four_Interface;


    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
