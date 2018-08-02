#ifndef SERIAL_CONFIG_H
#define SERIAL_CONFIG_H

#include <wiringPi.h>
#include <wiringSerial.h>
#include <QMessageBox>
#include <QString>
#include <iostream>
#include<sys/stat.h>
#include<unistd.h>
#include <QThread>

#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>

typedef struct node{
    QString Receive_Buffer;//Store received data
    QString Com_Data;//Command data label
    bool Empty_Sign;//Whether it is blank flag
    unsigned int Com_Num;//The number of data received
    struct node *Next;
}Node,*pNode;

class serial_config:public QObject
{
    Q_OBJECT
public:
    explicit serial_config();
    ~serial_config();
    pNode Create_list(int n);//Create a linked list for storing received data
    void Clear_Buffer(pNode buf,int num);//Empty the linked list
    void open_serial(void);//open Serial
    void close_serial(void);//close Serial
    void Command_Recognition(pNode com);//Recognize the received command
    void Send_Command(QString str,int channel,int num,int time);//Send command function
    //QString Conversion(QString data);//Less than four characters converted to four characters
    QString Data_Collect_nature(void);//Extract natural potential
    QStringList Data_Collect(void);//Extract received data
signals:
    void Emit_On_Off_Signal(bool);//emit power on signal
public:
    bool Power_fre;//Frequency setting completion flag
    bool Power_AC_DC_Flag; //AC and DC flags
    bool Flag_Array[15];//Received MCU reply flag to terminate command
    bool Serial_Block;
    int Serial_Flag;//Serial port number of WiringPi
    QString Interface_Signal;
    pNode Execute_Command_Pointer,Stor_Command_Pointer;//Command list pointer
    pNode Traversing_Data_Pointer,Stor_Data_Pointer;//Data list pointer
 private:
    bool Power_Open;//Power on flag
};

extern serial_config *serial;//Global serial port structure pointer
#endif
