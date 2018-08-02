#ifndef DEVICE_H
#define DEVICE_H

#include <QWidget>
#include <QStatusBar>
#include <QProgressBar>
#include "keyboard.h"


namespace Ui {
class device;
}

class device : public QWidget
{
    Q_OBJECT

public:
    explicit device(QWidget *parent = 0);
    ~device();
signals:
    void Send_Coefficient(double);
    void Send_Class(int);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_comboBox_activated(int index);
    void Recieve_Coefficient(QString data);

private:
    double pi=3.141592654;
    double D_Coefficient;
    QString Co_Flag;
    keyboard *key;
    Ui::device *ui;
};

#endif // DEVICE_H
