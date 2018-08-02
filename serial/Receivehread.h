#ifndef RECEIVEHREAD_H
#define RECEIVEHREAD_H

#include <QThread>
#include <QString>
#include "serial_config.h"

class receivehread:public QThread
{
public:
    receivehread(QObject *par=0);
    ~receivehread();
    QString Receive_Serial(int fd);
    QString Get_Com(QString data);
protected:
    void run();
};
#endif // RECEIVEHREAD_H
