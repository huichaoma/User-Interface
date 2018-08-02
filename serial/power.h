#ifndef POWER_H
#define POWER_H

#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QStatusBar>
#include <QProgressBar>
#include <QString>
#include "serial_config.h"
#include "keyboard.h"
#include "Sendthread.h"

namespace Ui {
class power;
}

class power : public QMainWindow
{
    Q_OBJECT

public:
    explicit power(QWidget *parent = 0);
    ~power();
private slots:
    void on_pushButton_clicked();
    void receiveData(QString data);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void Receive_Power_On_Off(bool sign);

private:
    QString Conversion(QString data);

private:
    QLabel *per1;
    keyboard *k1;
    Ui::power *ui;
};
#endif // POWER_H
