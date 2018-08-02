#ifndef FOUR_ELECTRODES_H
#define FOUR_ELECTRODES_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTextStream>
#include <QProgressBar>
#include <QAction>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QThread>
#include "serial_config.h"
#include "keyboard.h"
#include "power.h"
#include "Sendthread.h"
#include "qcustomplot.h"
#include "device_co.h"

namespace Ui {
class four_electrodes;
}

class four_electrodes : public QMainWindow
{
    Q_OBJECT

public:
    explicit four_electrodes(QWidget *parent = 0);
    ~four_electrodes();
private slots:
    void  open_window();
    void  save();
    void  new_row();
    void  new_col();
    void  delete_row();
    void  delete_col();
    void  clear_data();
    void  device_Coefficient();
    void  count();
    void  mean();
    void  plot_graphs();
    void on_comboBox_activated(const QString &arg1);
    void on_comboBox_2_activated(const QString &arg1);
    void quit();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void receiveData(QString data);
    void on_pushButton_3_clicked();
    void Receive_Data(bool sign);
    void Receive_Data_Filled(bool sign);
    void Receive_Co(double data);
    void Receive_Class(int Co);

private:
    int Row,Col;
    int Dev_Class;
    double Coefficient;
    int Col_data,Row_data;//Position of data
    void closeEvent(QCloseEvent *event);
    void Fill_Data(QStringList data);

    QLabel *per1;
    keyboard *k;
    device_Co *dev;
    Ui::four_electrodes *ui;

};

#endif // FOUR_ELECTRODES_H
