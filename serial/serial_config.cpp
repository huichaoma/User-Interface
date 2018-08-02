#include "serial_config.h"

serial_config *serial= new serial_config();

serial_config::serial_config()
{
    Power_Open=false;
    Power_fre=false;
    Power_AC_DC_Flag=false;
    Serial_Flag=0;
    Serial_Block=false;
    for(int i=0;i<15;i++)
        Flag_Array[i]=true;
}
serial_config::~serial_config()
{
    delete Execute_Command_Pointer;
    delete Stor_Command_Pointer;
    delete Traversing_Data_Pointer;
    delete Stor_Data_Pointer;
}
pNode serial_config::Create_list(int n)
{
    pNode temp;
    pNode head=new Node;

    head->Empty_Sign=false;
    head->Com_Num=0;
    temp=head;
    temp->Next=temp;

    for(int i=0;i<n;i++)
    {
        pNode newNode=new Node;
        newNode->Empty_Sign=false;
        newNode->Com_Num=0;
        temp->Next=newNode;

        temp=newNode;
        temp->Next=head;
    }

    return head;
}
void serial_config::Clear_Buffer(pNode buf,int num)
{
    pNode tem=buf;
    for(int i=0;i<num;i++)
    {    
        tem->Receive_Buffer.clear();
        tem->Com_Data.clear();
        tem->Empty_Sign=false;
        tem->Com_Num=0;
        tem=tem->Next;
    }
}
void serial_config::open_serial()
{
   while(wiringPiSetupSys()<0) ;
   if((Serial_Flag=serialOpen("/dev/ttyUSB0",115200))<0)
   {
       QMessageBox::warning(NULL,"提示","串口没有打开");
   }

}
void serial_config::close_serial()
{
    serialClose(Serial_Flag);
}
void serial_config::Command_Recognition(pNode com)
{
    if((com->Receive_Buffer.indexOf(QStringLiteral("1234"),0)!=-1)&&(!(Flag_Array[0])))
    {
        //close_serial();
        QMessageBox::information(NULL,"提示","重置MCU成功",QMessageBox::NoButton);
        Flag_Array[0]=true;
        Serial_Block=false;
        //open_serial();
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("00NN"),0)!=-1)&&(!(Flag_Array[1])))
    {
        QMessageBox::information(NULL,"提示","打开电源成功",QMessageBox::NoButton);
        Power_Open=true;
        Flag_Array[1]=true;
        emit Emit_On_Off_Signal(Power_Open);
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("00FF"),0)!=-1)&&(!(Flag_Array[2])))
    {
        QMessageBox::information(NULL,"提示","关闭电源成功",QMessageBox::NoButton);
        Power_Open=false;
        Power_fre=false;
        Flag_Array[2]=true;
        emit Emit_On_Off_Signal(Power_Open);
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("150V"),0)!=-1)&&(!(Flag_Array[3])))
    {
        QMessageBox::information(NULL,"提示","150V量程选择成功",QMessageBox::NoButton);
        Flag_Array[3]=true;
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("010V"),0)!=-1)&&(!(Flag_Array[4])))
    {
        QMessageBox::information(NULL,"提示","10V量程选择成功",QMessageBox::NoButton);
        Flag_Array[4]=true;
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("015A"),0)!=-1)&&(!(Flag_Array[5])))
    {
        QMessageBox::information(NULL,"提示","15A量程选择成功",QMessageBox::NoButton);
        Flag_Array[5]=true;
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("001A"),0)!=-1)&&(!(Flag_Array[6])))
    {
        QMessageBox::information(NULL,"提示","1A量程选择成功",QMessageBox::NoButton);
        Flag_Array[6]=true;
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("10mA"),0)!=-1)&&(!(Flag_Array[7])))
    {
        QMessageBox::information(NULL,"提示","10mA量程选择成功",QMessageBox::NoButton);
        Flag_Array[7]=true;
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("00AC"),0)!=-1)&&(!(Flag_Array[8])))
    {
        QMessageBox::information(NULL,"提示","交流设置成功",QMessageBox::NoButton);
        Power_AC_DC_Flag=true;
        Power_fre=true;
        Flag_Array[8]=true;
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("00DC"),0)!=-1)&&(!(Flag_Array[9])))
    {
        QMessageBox::information(NULL,"提示","直流设置成功",QMessageBox::NoButton);
        Power_AC_DC_Flag=false;
        Power_fre=true;
        Flag_Array[9]=true;
        Serial_Block=false;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("NANA"),0)!=-1)&&(!(Flag_Array[10])))
    {
        QMessageBox::information(NULL,"提示","数据测量，请等待。。。",QMessageBox::NoButton);
        Flag_Array[10]=true;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("MPMP"),0)!=-1)&&(!(Flag_Array[11])))
    {
        QMessageBox::information(NULL,"提示","数据测量，请等待。。。",QMessageBox::NoButton);
        Flag_Array[11]=true;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("CHAG"),0)!=-1)&&(!(Flag_Array[12])))
    {
        QMessageBox::information(NULL,"提示","数据测量，请等待。。。",QMessageBox::NoButton);
        Flag_Array[12]=true;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("WNAW"),0)!=-1)&&(!(Flag_Array[13])))
    {
        QMessageBox::information(NULL,"提示","数据测量，请等待。。。",QMessageBox::NoButton);
        Flag_Array[13]=true;
    }
    else if((com->Receive_Buffer.indexOf(QStringLiteral("WMPW"),0)!=-1)&&(!(Flag_Array[14])))
    {
        QMessageBox::information(NULL,"提示","数据测量，请等待。。。",QMessageBox::NoButton);
        Flag_Array[14]=true;
    }
}
void serial_config::Send_Command(QString str,int channel,int num,int time)
{
    Serial_Block=true;
    QByteArray tem=str.toLatin1();
    for(int i=0;i<num;i++)
    {
        serialPuts(Serial_Flag,tem.data());
        QThread::msleep(time);
        Flag_Array[channel]=false;
    }
}
/*QString serial_config::Conversion(QString data)
{
    unsigned int len=data.length(),len_1=0;
    QString tem="0000";
    if(len<=4)
    {
        len_1=4-len;
        tem.replace(len_1,len,data);
    }
    return tem;
}*/
QString serial_config::Data_Collect_nature(void)
{
    QString Data;
    int Position_Head=-1,Position_End=-1;
    bool Complete_Flag=false;
    for(int j=0;j<20;j++)
    {
        if(Traversing_Data_Pointer->Com_Num==1)
        {
            Complete_Flag=true;
            break;
        }
        else
            Clear_Buffer(Traversing_Data_Pointer,1);
        Traversing_Data_Pointer=Traversing_Data_Pointer->Next;
    }
    if(Complete_Flag)
    {
       Position_Head=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("V"),0);
       Position_End=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("S"),0);
       Data=(Traversing_Data_Pointer->Receive_Buffer).mid(Position_Head+1,Position_End-Position_Head-1);
       Clear_Buffer(Traversing_Data_Pointer,1);
       Traversing_Data_Pointer=Traversing_Data_Pointer->Next;
    }
    return Data;
}
QStringList serial_config::Data_Collect(void)
{
    QStringList Data;
    int Position_Head=-1,Position_End=-1;
    bool Complete_Flag=false;
    for(int j=0;j<20;j++)
    {
       if(Traversing_Data_Pointer->Com_Num>1)
       {
          Complete_Flag=true;
          break;
       }
       else
          Clear_Buffer(Traversing_Data_Pointer,1);
       Traversing_Data_Pointer=Traversing_Data_Pointer->Next;
    }
    if(Complete_Flag)
    {
        Position_Head=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("V"),0);
        Position_End=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("S"),0);
        if(Position_Head<Position_End)
        {
            Data<<(Traversing_Data_Pointer->Receive_Buffer).mid(Position_Head+1,Position_End-Position_Head-1);
            (Traversing_Data_Pointer->Receive_Buffer).remove(0,Position_End+1);
            Position_Head=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("I"),0);
            Position_End=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("S"),0);
            if(Position_Head<Position_End)
            {
                Data<<(Traversing_Data_Pointer->Receive_Buffer).mid(Position_Head+1,Position_End-Position_Head-1);
                if(Power_AC_DC_Flag)
                {
                    (Traversing_Data_Pointer->Receive_Buffer).remove(0,Position_End+1);
                    Position_Head=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("P"),0);
                    Position_End=(Traversing_Data_Pointer->Receive_Buffer).indexOf(QStringLiteral("S"),0);
                    if(Position_Head<Position_End)
                    {
                        Data<<(Traversing_Data_Pointer->Receive_Buffer).mid(Position_Head+1,Position_End-Position_Head-1);
                    }
                }
            }
        }
        Clear_Buffer(Traversing_Data_Pointer,1);
        Traversing_Data_Pointer=Traversing_Data_Pointer->Next;
    }
    return Data;
}
