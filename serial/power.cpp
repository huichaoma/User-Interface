#include "power.h"
#include "ui_power.h"



power::power(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::power)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    this ->setFixedSize(500,350);
    statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
    QLabel *per1 = new QLabel("电源频率参数配置！", this);
    statusBar()->addWidget(per1);
    connect(serial,SIGNAL(Emit_On_Off_Signal(bool)),this,SLOT(Receive_Power_On_Off(bool)));
    if(serial->Power_fre)
        ui->pushButton_3->setText("关闭");
}
power::~power()
{
    delete per1;
    delete k1;
    delete ui;
}

void power::on_pushButton_clicked()
{

   QString tem_1="01DA";
   tem_1+=power::Conversion(ui->pushButton_2->text());
   tem_1+="ST";
   QByteArray tem_2=tem_1.toLatin1();
   char *tem_3=tem_2.data();
   if(ui->pushButton_3->text()=="打开")
       QMessageBox::warning(NULL,"警告","请打开电源");
   else
   {
       serial->Flag_Array[8]=false;
       serial->Flag_Array[9]=false;
       for(int i=0;i<1;i++)
       {
           serialPuts(serial->Serial_Flag,tem_3);
           QThread::msleep(200);
           if(serial->Flag_Array[8]||serial->Flag_Array[9])
               break;
       }
       this->hide();
   }
}
void power::on_pushButton_2_clicked()
{
    k1=new keyboard();
    connect(k1,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    k1->show();
}
void power::receiveData(QString data)
{
    ui->pushButton_2->setText(data);
}
QString power::Conversion(QString data)
{
    unsigned int len=data.length(),len_1=0;
    double da=data.toDouble();
    QString tem="0000";
    if(da>=1)
    {
        if(len<=4)
        {
            len_1=4-len;
            tem.replace(len_1,len,data);
        }
    }
    else if(da>=0)
    {
        if(len<=4)
        {
            tem.replace(0,len,data);
        }
    }
    return tem;
}

void power::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text()=="关闭")
    {
        serial->Send_Command("01DA00FFST\0",2,1,300);
    }
    else
    {
        serial->Send_Command("01DA00NNST\0",1,1,300);
    }
}
void power::Receive_Power_On_Off(bool sign)
{
   if(sign)
      ui->pushButton_3->setText("关闭");
   else
      ui->pushButton_3->setText("打开");
}
