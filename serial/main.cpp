#include <QApplication>
#include "mainwindow.h"
#include "Sendthread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    serial->open_serial();
    serial->Execute_Command_Pointer=serial->Stor_Command_Pointer=serial->Create_list(9);
    serial->Traversing_Data_Pointer=serial->Stor_Data_Pointer=serial->Create_list(19);
    new_thread->start();
    w.show();
    return a.exec();
}
