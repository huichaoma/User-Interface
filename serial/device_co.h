#ifndef DEVICE_CO_H
#define DEVICE_CO_H

#include <QDialog>
#include "keyboard.h"

namespace Ui {
class device_Co;
}

class device_Co : public QDialog
{
    Q_OBJECT

public:
    explicit device_Co(QWidget *parent = 0);
    ~device_Co();
signals:
    void Send_Coefficient(double);
    void Send_Class(int);

private slots:
    void on_pushButton_7_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_comboBox_activated(int index);
    void Recieve_Coefficient(QString data);

private:
    double pi=3.141592654;
    double D_Coefficient;
    QString Co_Flag;
    keyboard *key;
    Ui::device_Co *ui;
};

#endif // DEVICE_CO_H
