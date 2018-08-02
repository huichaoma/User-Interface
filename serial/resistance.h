#ifndef RESISTANCE_H
#define RESISTANCE_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QProgressBar>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include "serial_config.h"
#include "keyboard.h"
#include "power.h"
#include "Sendthread.h"
#include "qcustomplot.h"
#include <QThread>

namespace Ui {
class resistance;
}

class resistance : public QMainWindow
{
    Q_OBJECT

public:
    explicit resistance(QWidget *parent = 0);
    ~resistance();
private slots:
    void  open_window();
    void  save();
    void  new_row();
    void  new_col();
    void  delete_row();
    void  delete_col();
    void  clear_data();
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

private:
    int Row,Col;
    int Col_data,Row_data;//Position of data
    void  closeEvent(QCloseEvent *event);
    void  Fill_Data(QStringList data);

    QLabel *per1;
    keyboard *k;
    Ui::resistance *ui;
};

#endif // RESISTANCE_H
