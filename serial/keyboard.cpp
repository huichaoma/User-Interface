#include "keyboard.h"
#include "ui_keyboard.h"


keyboard::keyboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keyboard)
{
    ui->setupUi(this);
}

keyboard::~keyboard()
{
    delete ui;
}

void keyboard::on_pushButton_clicked()
{
   Key+="1";
   ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_2_clicked()
{
    Key+="2";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_3_clicked()
{
    Key+="3";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_4_clicked()
{
    Key+="+";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_5_clicked()
{
    Key+="4";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_6_clicked()
{
    Key+="5";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_7_clicked()
{
    Key+="6";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_8_clicked()
{
    Key+="-";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_9_clicked()
{
    Key+="7";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_10_clicked()
{
    Key+="8";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_11_clicked()
{
    Key+="9";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_12_clicked()
{
    Key+="*";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_13_clicked()
{
    Key+=".";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_14_clicked()
{
    Key+="0";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_15_clicked()
{
    Key=Key.left(Key.length()-1);
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_16_clicked()
{
    Key+="/";
    ui->lineEdit->setText(Key);
}

void keyboard::on_pushButton_17_clicked()
{
    Key.clear();
    this->close();
}

void keyboard::on_pushButton_18_clicked()
{
    emit sendData(Key);
    this->close();
}
