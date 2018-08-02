#ifndef NATURE_H
#define NATURE_H

#include <QMainWindow>
#include <QWidget>
#include <QMenuBar>
#include <QStatusBar>
#include <QProgressBar>
#include <QAction>
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QComboBox>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QDateTime>
#include "serial_config.h"
#include "keyboard.h"
#include "Sendthread.h"
#include <QThread>
#include "qcustomplot.h"

namespace Ui {
class nature;
}

class nature : public QMainWindow
{
    Q_OBJECT
public:
    explicit nature(QWidget *parent = 0);
    ~nature();
 private slots:
    void  open_window();
    void  save();
    void  new_row();
    void  new_col();
    void  delete_row();
    void  delete_col();
    void  clear_data();
    void  mean();
    void  plot_graphs();
    void on_comboBox_2_activated(const QString &arg1);
    void quit();
    void on_pushButton_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_pushButton_2_clicked();
    void receiveData(QString data);
    void Receive_Data(bool sign);
    void Data_Filled(bool sign);
private:
    int Row,Col;
    int Col_data,Row_data;//Position of data
    void Fill_Data(QString str);
    void    closeEvent(QCloseEvent *event);


    QLabel *per1;
    keyboard *k;
    Ui::nature *ui;
};


#endif // NATURE_H
