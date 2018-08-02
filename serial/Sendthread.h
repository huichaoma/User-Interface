#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QString>
#include <QByteArray>
#include "serial_config.h"

class SendThread:public QThread
{
    Q_OBJECT
public:
    explicit SendThread(QObject *parent=0);
    ~SendThread();
    void Send_Com(void);//Send inquiry battery command
    QString Receive_Serial(int fd);//Receive data, return string
    QString Get_Com(QString data);//Label the data received
    void Receive_Data(void);//Receive data and store
    void stop();//Stop new thread
signals:
    void Power_Signal(int);//emit received power
    void Com_Signal(bool);//emit received command signal
    void Command_Block_Emit(bool);//emit command blocked
    void Data_Nat_Signal(bool);//emit received nature data signal
    void Data_Res_Signal(bool);//emit received reisitance data signal
    void Data_Fou_Signal(bool);//emit received resistivity data signal
    void Nat_Block_Emit(bool);//emit nature data blocked
    void Res_Block_Emit(bool);//emit resistance data blocked
    void Fou_Block_Emit(bool);//emit resistivity data blocked
private:
    int Power_1; //Received power value
    bool Thread_Sign;//Process flag
protected:
    void run();//Process execution function
};

extern SendThread *new_thread;//Global new thread

#endif // SENDTHREAD_H
