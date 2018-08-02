#include "about.h"
#include "ui_dialog.h"
#include <Qt>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("关于");
    this->setFixedSize(420, 250);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
   this->close();
}
