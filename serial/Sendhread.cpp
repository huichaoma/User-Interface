#include "Sendthread.h"
#include <QtDebug>

SendThread *new_thread= new SendThread();//Create new thread for receiving data

SendThread::SendThread(QObject *parent):QThread(parent)
{
    Power_1=0;
    Thread_Sign=true;
}
SendThread::~SendThread()
{

}
void SendThread::Send_Com(void)
{
    serialPuts(serial->Serial_Flag,"01COPOWEST\0");
}
QString SendThread::Receive_Serial(int fd)
{
    QByteArray str;
    int i=0;
    while(serialDataAvail(fd))
        str[i++]=serialGetchar(fd);
    str[i]='\0';
    return QString(str);
}
QString SendThread::Get_Com(QString data)
{
    if(data.count(QStringLiteral("E")))
    {
        //serialPuts(serial->Serial_Flag,"Return\n");
        //msleep(20);
        return QStringLiteral("RE");
    }
    else
    {
        if(data.count(QStringLiteral("V")))
        {
            if(data.count(QStringLiteral("I")))
            {
                if(data.count(QStringLiteral("P")))
                {
                    //serialPuts(serial->Serial_Flag,"AC\n");
                    //msleep(20);
                    return QStringLiteral("MVMIMP");
                }
                else
                {
                    //serialPuts(serial->Serial_Flag,"DC\n");
                    //msleep(20);
                    return QStringLiteral("MVMI");
                }
             }
             else
             {
                //serialPuts(serial->Serial_Flag,"Nature\n");
                //msleep(20);
                return QStringLiteral("MV");
             }
        }
        else
            return NULL;
    }
}
void SendThread::Receive_Data(void)
{
    QString tem;
    int num_com=0;
    int Head_P=-1,End_P=-1;
    tem=Receive_Serial(serial->Serial_Flag);
    serialFlush(serial->Serial_Flag);
    //QByteArray tem_2=tem.toLatin1();
    //serialPuts(serial->Serial_Flag,tem_2.data());
    //msleep(20);
    if(!tem.isEmpty())
    {
        num_com=tem.count('S');
        if(num_com!=0)
        {
            if(tem.count(QStringLiteral("O")))//Update power in real time
            {
                Head_P=tem.indexOf(QStringLiteral("O"),0);
                End_P=tem.indexOf(QStringLiteral("S"),0);
                Power_1=(int)(((tem.mid(Head_P+1,End_P-Head_P-1).toDouble())-9.0)/(13.9-9.0)*100);
                emit Power_Signal(Power_1);
                //serialPuts(serial->Serial_Flag,"Power\n");
                //msleep(20);
            }
            else
            {
                QString com=Get_Com(tem);
                 if(!com.isEmpty())
                 {
                     //serialPuts(serial->Serial_Flag,com.toLatin1().data());
                     //msleep(20);
                    if(!QString::compare(com,"RE"))
                    {
                        if(num_com==1)
                        {
                            if((!(serial->Stor_Command_Pointer->Empty_Sign)))
                            {
                                serial->Stor_Command_Pointer->Receive_Buffer=tem;
                                serial->Stor_Command_Pointer->Com_Data=com;
                                serial->Stor_Command_Pointer->Com_Num=num_com;
                                serial->Stor_Command_Pointer=serial->Stor_Command_Pointer->Next;
                                emit Com_Signal(true);
                                //serialPuts(serial->Serial_Flag,(serial->Execute_Command_Pointer->Com_Data).toLatin1().data());
                                //msleep(20);
                                //serialPuts(serial->Serial_Flag,(serial->Execute_Command_Pointer->Receive_Buffer).toLatin1().data());
                                //msleep(20);
                                //serial->Execute_Command_Pointer=serial->Execute_Command_Pointer->Next;
                            }
                            else
                            {
                                emit Command_Block_Emit(true);
                            }
                       }
                    }
                    else
                    {
                        if(num_com==(com.length()/2))
                        {
                            if(!(serial->Stor_Data_Pointer->Empty_Sign))
                            {
                                serial->Stor_Data_Pointer->Receive_Buffer=tem;
                                serial->Stor_Data_Pointer->Com_Data=com;
                                serial->Stor_Data_Pointer->Com_Num=num_com;
                                serial->Stor_Data_Pointer=serial->Stor_Data_Pointer->Next;
                                if(!QString::compare(serial->Interface_Signal,"Nat"))
                                    emit Data_Nat_Signal(true);
                                else if(!QString::compare(serial->Interface_Signal,"Res"))
                                    emit Data_Res_Signal(true);
                                else if(!QString::compare(serial->Interface_Signal,"Fou"))
                                    emit Data_Fou_Signal(true);
                                //serialPuts(serial->Serial_Flag,(serial->Traversing_Data_Pointer->Com_Data).toLatin1().data());
                                //msleep(20);
                                //serialPuts(serial->Serial_Flag,(serial->Traversing_Data_Pointer->Receive_Buffer).toLatin1().data());
                                //msleep(20);
                                //serial->Traversing_Data_Pointer=serial->Traversing_Data_Pointer->Next;
                            }
                            else
                            {
                                if(!QString::compare(serial->Interface_Signal,"Nat"))
                                    emit Nat_Block_Emit(true);
                                else if(!QString::compare(serial->Interface_Signal,"Res"))
                                    emit Res_Block_Emit(true);
                                else if(!QString::compare(serial->Interface_Signal,"Fou"))
                                    emit Fou_Block_Emit(true);
                            }
                        }
                        serial->Serial_Block=false;
                    }
                  }
            }
            num_com=0;
        }
        tem.clear();
    }
}
void SendThread::run()
{
    int data_flag;
    int time=0;
    while(Thread_Sign)
    {
       time=200;
       while(time>0)
       {
            msleep(50); //wait for the command to be stored in the buffer
            data_flag=serialDataAvail(serial->Serial_Flag);
            if(data_flag)
            {
                Receive_Data();
                data_flag=0;
            }
            time--;
       }
       if(!serial->Serial_Block)
            Send_Com();
    }
}
void SendThread::stop()
{
    Thread_Sign=false;
}
