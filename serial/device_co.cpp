#include "device_co.h"
#include "ui_device_co.h"

device_Co::device_Co(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::device_Co)
{
    ui->setupUi(this);
    D_Coefficient=0;
    key=new keyboard();
    connect(key,SIGNAL(sendData(QString)),this,SLOT(Recieve_Coefficient(QString)));
}

device_Co::~device_Co()
{
    delete key;
    delete ui;
}


void device_Co::on_comboBox_activated(int index)
{
    emit Send_Class(index);
}

void device_Co::on_pushButton_7_clicked()
{
    double AM=ui->pushButton->text().toDouble();
    double AN=ui->pushButton_2->text().toDouble();
    double BM=ui->pushButton_4->text().toDouble();
    double BN=ui->pushButton_5->text().toDouble();
    double n=ui->pushButton_3->text().toDouble();
    double MN=ui->pushButton_6->text().toDouble();

    if(ui->comboBox->currentText()=="二电极")
    {
        D_Coefficient=2.0*pi*AM;
    }
    else if(ui->comboBox->currentText()=="三、四电极")
    {
        D_Coefficient=2.0*pi*AM*AN/MN;
    }
    else if(ui->comboBox->currentText()=="偶电极")
    {
        D_Coefficient=pi*AM*n*(n+1)*(n+2);
    }
    else if(ui->comboBox->currentText()=="中间梯度")
    {
        D_Coefficient=2.0*pi*AM*AN*BM*BN/(MN*AM*AN+MN*BM*BN);
    }
    emit Send_Coefficient(D_Coefficient);
    this->hide();
}

void device_Co::on_pushButton_clicked()
{
    Co_Flag.clear();
    Co_Flag="AM";
    key->show();
}

void device_Co::on_pushButton_2_clicked()
{
    Co_Flag.clear();
    Co_Flag="AN";
    key->show();
}

void device_Co::on_pushButton_4_clicked()
{
    Co_Flag.clear();
    Co_Flag="BM";
    key->show();
}

void device_Co::on_pushButton_5_clicked()
{
    Co_Flag.clear();
    Co_Flag="BN";
    key->show();
}

void device_Co::on_pushButton_3_clicked()
{
    Co_Flag.clear();
    Co_Flag="n";
    key->show();
}

void device_Co::on_pushButton_6_clicked()
{
    Co_Flag.clear();
    Co_Flag="MN";
    key->show();
}
void device_Co::Recieve_Coefficient(QString data)
{
    if(!QString::compare(Co_Flag,"AM"))
    {
       ui->pushButton->setText(data);
    }
    else if(!QString::compare(Co_Flag,"AN"))
    {
       ui->pushButton_2->setText(data);
    }
    else if(!QString::compare(Co_Flag,"BM"))
    {
       ui->pushButton_4->setText(data);
    }
    else if(!QString::compare(Co_Flag,"BN"))
    {
       ui->pushButton_5->setText(data);
    }
    else if(!QString::compare(Co_Flag,"n"))
    {
       ui->pushButton_3->setText(data);
    }
    else if(!QString::compare(Co_Flag,"MN"))
    {
       ui->pushButton_6->setText(data);
    }
}
