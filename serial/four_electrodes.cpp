#include "four_electrodes.h"
#include "ui_four_electrodes.h"



four_electrodes::four_electrodes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::four_electrodes)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
    per1 = new QLabel("四电极测量电阻率", this);
    statusBar()->addWidget(per1);
    
    QMenu *file_1=menuBar()->addMenu(QObject::tr("  菜  单  "));
    QAction *open_window= new QAction(QObject::tr("  打  开  "),parent);
    QAction *save=new QAction(QObject::tr("  保  存  "),parent);
    QAction *quit=new QAction(QObject::tr("  退  出  "),parent);
    file_1->addAction(open_window);
    file_1->addAction(save);
    file_1->addAction(quit);
    connect(open_window,&QAction::triggered,this,&four_electrodes::open_window);
    connect(save,&QAction::triggered,this,&four_electrodes::save);
    connect(quit,&QAction::triggered,this,&four_electrodes::quit);
    QMenu *basic= menuBar()->addMenu(QObject::tr("  基本操作  "));
    QAction *new_row= new QAction(QObject::tr("  新添一行  "),parent);
    QAction *new_col= new QAction(QObject::tr("  新添一列  "),parent);
    QAction *delete_row= new QAction(QObject::tr("  删除一行  "),parent);
    QAction *delete_col= new QAction(QObject::tr("  删除一列  "),parent);
    QAction *clear= new QAction(QObject::tr("  清空数据  "),parent); 
    basic->addAction(new_row);
    basic->addAction(new_col);
    basic->addAction(delete_row);
    basic->addAction(delete_col);
    basic->addAction(clear);
    connect(new_row,&QAction::triggered,this,&four_electrodes::new_row);
    connect(new_col,&QAction::triggered,this,&four_electrodes::new_col);
    connect(delete_row,&QAction::triggered,this,&four_electrodes::delete_row);
    connect(delete_col,&QAction::triggered,this,&four_electrodes::delete_col);
    connect(clear,&QAction::triggered,this,&four_electrodes::clear_data);
    QMenu *formula= menuBar()->addMenu(QObject::tr("  计  算  "));
    QAction *device= new QAction(QObject::tr("  基本配置  "),parent);
    QAction *count= new QAction(QObject::tr("  计  算  "),parent);
    QAction *mean= new QAction(QObject::tr("  平  均  "),parent);
    formula->addAction(device);
    formula->addAction(count);
    formula->addAction(mean);
    connect(device,&QAction::triggered,this,&four_electrodes::device_Coefficient);
    connect(count,&QAction::triggered,this,&four_electrodes::count);
    connect(mean,&QAction::triggered,this,&four_electrodes::mean);
    QMenu *draw=menuBar()->addMenu(QObject::tr("  绘  图  "));
    QAction *plot_graphs= new QAction(QObject::tr("画曲线图"),parent);
    draw->addAction(plot_graphs);
    connect(plot_graphs,&QAction::triggered,this,&four_electrodes::plot_graphs);
    ui->tableWidget->setColumnCount(16);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue}");
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()
                                               <<"电压1"<<"电流1"<<"相位1"<<"电阻率1"
                                               <<"电压2"<<"电流2"<<"相位2"<<"电阻率2"
                                               <<"电压3"<<"电流3"<<"相位3"<<"电阻率3"
                                               <<"电压4"<<"电流4"<<"相位4"<<"电阻率4");
    ui->tableWidget->setRowCount(500);
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue}");
    Col_data=0,Row_data=0;

    connect(new_thread,SIGNAL(Data_Fou_Signal(bool)),this,SLOT(Receive_Data(bool)));
    connect(new_thread,SIGNAL(Fou_Block_Emit(bool)),this,SLOT(Receive_Data_Filled(bool)));
    serial->Traversing_Data_Pointer=serial->Stor_Data_Pointer;
}
four_electrodes::~four_electrodes()
{
    delete per1;
    delete ui;
    delete k;
    delete dev;
}
void four_electrodes::open_window()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, tr("open file"),"",tr("Files (*.csv)"));
    ui->tableWidget->clear();
    QDir dir=QDir::current();
    QFile file(dir.filePath(fileName));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"Open file failed!";
    if(fileName.isEmpty())
        return ;
    QTextStream *out=new QTextStream(&file);
    QStringList tempOption=out->readAll().split("\n");
    for(int i=0;i<tempOption.count();i++)
    {
        QStringList tembar= tempOption.at(i).split(",");
        if(tempOption.count()>500)
            ui->tableWidget->setRowCount(tempOption.count());
        if(tembar.count()>10)
            ui->tableWidget->setColumnCount(tembar.count());
        if(i == 0)
        {
             ui->tableWidget->setHorizontalHeaderLabels(tembar);
        }
        else
        {
            for(int j=0;j<tembar.count();j++)
            {
                ui->tableWidget->setItem(i-1, j, new QTableWidgetItem(tembar.at(j)));
            }
        }
     }
    file.close();
}
void four_electrodes::save()
{
    QString current_date =QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
    current_date+=".csv";
    QString fileName=QFileDialog::getSaveFileName(NULL,tr("Save file"),current_date,tr("files(*.csv)"));
    if(fileName.isEmpty())
        return ;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
       std::cerr<<"Cannot open file for wirting:"<<qPrintable(file.errorString())<<std::endl;
       return;
    }
    QTextStream stream(&file);
    QString contents;
    QHeaderView *header=ui->tableWidget->horizontalHeader();
    int Col_non_null=0,Row_non_null=0;
    for(int m=0;m<ui->tableWidget->columnCount();m++)
    {
        for(int n=0;n<ui->tableWidget->rowCount();n++)
        {
            if(ui->tableWidget->item(n,m))
            {
                Col_non_null=m;
                if(n>Row_non_null)
                {
                    Row_non_null=n;
                }
            }
        }
    }
    if(header)
    {
        for(int i=0;i<Col_non_null+1;i++)
        {
            QTableWidgetItem *item=ui->tableWidget->horizontalHeaderItem(i);
            contents+=item->text()+",";
        }
        contents+="\n";
    }
    for(int i=0;i<Row_non_null+1;i++)
    {
        for(int j=0;j<Col_non_null+1;j++)
        {
            QTableWidgetItem *item=ui->tableWidget->item(i,j);
            QString str;
            if(item)
               str=item->text();
            else
               str=" ";
            contents+=str+",";
        }
        contents+="\n";
    }
    stream<<contents;
    file.close();
}
void four_electrodes::new_row()
{
    int Row_count=ui->tableWidget->rowCount()+1;
    ui->tableWidget->setRowCount(Row_count);
}
void four_electrodes::new_col()
{
    int Col_count=ui->tableWidget->columnCount()+4;
    QStringList header;
    ui->tableWidget->setColumnCount(Col_count);
    for(int i=0;i<(Col_count/4);i++)
       {
        QString t="电压"+QString::number(i+1,10);
        header<<t;
        t.clear();
        t="电流"+QString::number(i+1,10);
        header<<t;
        t.clear();
        t="相位"+QString::number(i+1,10);
        header<<t;
        t.clear();
        t="相位"+QString::number(i+1,10);
        header<<t;
       }
    ui->tableWidget->setHorizontalHeaderLabels(header);
}
void four_electrodes::delete_row()
{
    int Row_count=ui->tableWidget->rowCount()-1;
    ui->tableWidget->setRowCount(Row_count);
}
void four_electrodes::delete_col()
{
    int Col_count=ui->tableWidget->columnCount()-4;
    ui->tableWidget->setColumnCount(Col_count);
}
void four_electrodes::clear_data()
{
   ui->tableWidget->clearContents();
   Col_data=0,Row_data=0;
}
void four_electrodes::device_Coefficient()
{
    dev=new device_Co();
    connect(dev,SIGNAL(Send_Coefficient(double)),this,SLOT(Receive_Co(double)));
    connect(dev,SIGNAL(Send_Class(int)),this,SLOT(Receive_Class(int)));
    dev->show();
}
void four_electrodes::count()
{
    if((Col_data==0)&&(Row_data==0))
    {
        QMessageBox::warning(NULL,"警告","请先收集数据!");
    }
    else
    {
        for(int i=0;i<=Row_data;i++)
        {
            for(int j=0;j<=Col_data;)
            {
                if((ui->tableWidget->item(i,j)!=0)&&(ui->tableWidget->item(i,j+1)!=0))
                {
                    QString Data_V=ui->tableWidget->item(i,j)->text();
                    QString Data_I=ui->tableWidget->item(i,j+1)->text();
                    double Data_V_1=Data_V.toDouble();
                    double Data_I_1=Data_I.toDouble();
                    if(Data_I_1!=0)
                    {
                        double Resistance=Data_V_1/Data_I_1*Coefficient;
                        ui->tableWidget->setItem(i,j+3, new QTableWidgetItem(QString::number(Resistance, 10, 4)));
                    }
                    else
                    {
                        ui->tableWidget->setItem(i,j+3, new QTableWidgetItem("NaN"));
                    }

                }
                j=j+4;
            }
         }
    }
}
void four_electrodes::mean()
{

}
void four_electrodes::plot_graphs()
{

}
void four_electrodes::on_comboBox_activated(const QString &arg1)
{
    if(!QString::compare(arg1,QStringLiteral("15A")))
       serial->Send_Command("01CO015AST\0",5,1,300);
    else if(!QString::compare(arg1,QStringLiteral("1A")))
       serial->Send_Command("01CO001AST\0",6,1,300);
    else if(!QString::compare(arg1,QStringLiteral("10mA")))
        serial->Send_Command("01CO10mAST\0",7,1,300);
}

void four_electrodes::on_comboBox_2_activated(const QString &arg1)
{
    if(!QString::compare(arg1,QStringLiteral("10V")))
       serial->Send_Command("01CO010VST\0",4,1,300);
    else if(!QString::compare(arg1,QStringLiteral("150V")))
       serial->Send_Command("01CO150VST\0",3,1,300);
}

void four_electrodes::quit()
{
    serial->Clear_Buffer(serial->Traversing_Data_Pointer,20);
    this->hide();
}
void four_electrodes::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(NULL,tr("退出数据测量"),QString(tr("确认退出数据测量？")),QMessageBox::Yes | QMessageBox::No);
    if(button==QMessageBox::No){
        event->ignore();
    }
    else if(button==QMessageBox::Yes){
        event->accept();
    }

}
void four_electrodes::on_pushButton_clicked()
{
    serial->Send_Command("01COSAMPST\0",11,1,300);
}

void four_electrodes::on_pushButton_2_clicked()
{
    serial->Clear_Buffer(serial->Traversing_Data_Pointer,20);
    if(Row_data!=0)
    {
        Row_data=0;
        Col_data+=4;
    }
    serial->Send_Command("01COCHAGST\0",12,1,300);
}
void four_electrodes::on_tableWidget_cellDoubleClicked(int row, int column)
{
    k=new keyboard();
    connect(k,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    Row=row;
    Col=column;
    Row_data=row;
    Col_data=column;
    k->show();
}
void four_electrodes::receiveData(QString data)
{
    if(!data.isEmpty())
    {
        ui->tableWidget->setItem(Row, Col, new QTableWidgetItem(data));
        if(Row_data==(ui->tableWidget->rowCount()-1))
        {
           if(Col_data==ui->tableWidget->columnCount())
           {
              new_col();
           }
           Row_data=0;
           Col_data+=4;
        }
        else
        {
           Row_data+=1;
        }
        Col_data=(Col_data/4)*4;
    }
}

void four_electrodes::on_pushButton_3_clicked()
{
    serial->Clear_Buffer(serial->Traversing_Data_Pointer,20);
    if(Row_data!=0)
        Row_data+=1;
    serial->Send_Command("01COWMPWST\0",14,1,300);
}
void four_electrodes::Receive_Data(bool sign)
{
    if(sign)
    {
      QStringList data_1; 
      data_1=serial->Data_Collect();
      Fill_Data(data_1);
    }
    else
    {
        QMessageBox::warning(NULL,"警告","数据收集失败!");
    }
}
void four_electrodes::Receive_Data_Filled(bool sign)
{
    if(sign)
    {
        QStringList data_1;
        for(int i=0;i<20;i++)
        {
            data_1=serial->Data_Collect();
            Fill_Data(data_1);
        }
    }
}
void four_electrodes::Fill_Data(QStringList data)
{
    if(!data.isEmpty())
    {
        ui->tableWidget->setItem(Row_data, Col_data, new QTableWidgetItem(data.at(0)));
        ui->tableWidget->setItem(Row_data, Col_data+1, new QTableWidgetItem(data.at(1)));
        if(serial->Power_AC_DC_Flag)
            ui->tableWidget->setItem(Row_data, Col_data+2, new QTableWidgetItem(data.at(2)));
        if(Row_data==(ui->tableWidget->rowCount()-1))
        {
            if(Col_data==ui->tableWidget->columnCount())
            {
                new_col();
            }
            Row_data=0;
            Col_data+=4;
        }
        else
        {
            Row_data+=1;
        }
    }
}
void four_electrodes::Receive_Co(double data)
{
    Coefficient=data;
    QMessageBox::warning(NULL,"警告",QString::number(data,'f',10));
}
void four_electrodes::Receive_Class(int Co)
{
    Dev_Class=Co;
}
