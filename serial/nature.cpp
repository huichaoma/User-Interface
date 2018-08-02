#include "nature.h"
#include "ui_nature.h"

nature::nature(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::nature)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
    per1 = new QLabel("自然电位测量", this);
    statusBar()->addWidget(per1);

    QMenu *file_1=menuBar()->addMenu(QObject::tr("  菜  单  "));
    QAction *open_window= new QAction(QObject::tr("  打  开  "),parent);
    QAction *save=new QAction(QObject::tr("  保  存  "),parent);
    QAction *quit=new QAction(QObject::tr("  退  出  "),parent);
    file_1->addAction(open_window);
    file_1->addAction(save);
    file_1->addAction(quit);
    connect(open_window,&QAction::triggered,this,&nature::open_window);
    connect(save,&QAction::triggered,this,&nature::save);
    connect(quit,&QAction::triggered,this,&nature::quit);
    QMenu *basic= menuBar()->addMenu(QObject::tr("  基本操作  "));
    QAction *new_row= new QAction(QObject::tr("  新添一行  "),parent);
    QAction *new_col= new QAction(QObject::tr("  新添一列  "),parent);
    QAction *delete_row= new QAction(QObject::tr("  删除一行  "),parent);
    QAction *delete_col= new QAction(QObject::tr("  删除一列  "),parent);
    QAction *clear= new QAction(QObject::tr("  清除数据  "),parent);
    basic->addAction(new_row);
    basic->addAction(new_col);
    basic->addAction(delete_row);
    basic->addAction(delete_col);
    basic->addAction(clear);
    connect(new_row,&QAction::triggered,this,&nature::new_row);
    connect(new_col,&QAction::triggered,this,&nature::new_col);
    connect(delete_row,&QAction::triggered,this,&nature::delete_row);
    connect(delete_col,&QAction::triggered,this,&nature::delete_col);
    connect(clear,&QAction::triggered,this,&nature::clear_data);
    QMenu *formula= menuBar()->addMenu(QObject::tr("  计  算  "));
    QAction *mean= new QAction(QObject::tr("  平  均  "),parent);
    formula->addAction(mean);
    connect(mean,&QAction::triggered,this,&nature::mean);
    QMenu *draw=menuBar()->addMenu(QObject::tr("  绘  图  "));
    QAction *plot_graphs= new QAction(QObject::tr("画曲线图"),parent);
    draw->addAction(plot_graphs);
    connect(plot_graphs,&QAction::triggered,this,&nature::plot_graphs);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue}");
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"电位1"<<"电位2"<<"电位3"
                                               <<"电位4"<<"电位5"<<"电位6"<<"电位7"
                                               <<"电位8"<<"电位9"<<"电位10");
    ui->tableWidget->setRowCount(500);
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:skyblue}");

    connect(new_thread,SIGNAL(Data_Nat_Signal(bool)),this,SLOT(Receive_Data(bool)));
    connect(new_thread,SIGNAL(Nat_Block_Emit(bool)),this,SLOT(Data_Filled(bool)));
    serial->Traversing_Data_Pointer=serial->Stor_Data_Pointer;
    Col_data=0,Row_data=0;
}
nature::~nature()
{
    delete k;
    delete ui;
    delete per1;
}
void nature::open_window()
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
void nature::save()
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
void nature::new_row()
{
    int Row_count=ui->tableWidget->rowCount()+1;
    ui->tableWidget->setRowCount(Row_count);
}
void nature::new_col()
{
    int Col_count=ui->tableWidget->columnCount()+1;
    QStringList header;
    ui->tableWidget->setColumnCount(Col_count);
    for(int i=0;i<Col_count;i++)
       {
        QString t="电位"+QString::number(i+1,10);
         header<<t;
       }
    ui->tableWidget->setHorizontalHeaderLabels(header);
}
void nature::delete_row()
{
    int Row_count=ui->tableWidget->rowCount()-1;
    ui->tableWidget->setRowCount(Row_count);
}
void nature::delete_col()
{
    int Col_count=ui->tableWidget->columnCount()-1;
    ui->tableWidget->setColumnCount(Col_count);
}
void nature::clear_data()
{
    ui->tableWidget->clearContents();
    Col_data=0,Row_data=0;
}
void nature::mean()
{

}
void nature::plot_graphs()
{

}
void nature::on_comboBox_2_activated(const QString &arg1)
{
    if(!QString::compare(arg1,QStringLiteral("10V")))
       serial->Send_Command("01CO010VST\0",4,1,300);
    else if(!QString::compare(arg1,QStringLiteral("150V")))
       serial->Send_Command("01CO150VST\0",3,1,300);
}

void nature::quit()
{
     serial->Clear_Buffer(serial->Traversing_Data_Pointer,20);
     this->hide();
}
void nature::closeEvent(QCloseEvent *event)
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


void nature::on_pushButton_clicked()
{
    serial->Send_Command("01CONANAST\0",10,1,300);
}

void nature::on_tableWidget_cellDoubleClicked(int row, int column)
{
    k=new keyboard();
    connect(k,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    Row=row;
    Col=column;
    Row_data=row;
    Col_data=column;
    k->show();
}

void nature::on_pushButton_2_clicked()
{
    serial->Clear_Buffer(serial->Traversing_Data_Pointer,20);
    if(Row_data!=0)
        Row_data+=1;
    serial->Send_Command("01COWNAWST\0",13,1,300);
}
void nature::receiveData(QString data)
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
           Col_data+=1;
        }
        else
        {
           Row_data+=1;
        }
    }
}
void nature::Receive_Data(bool sign)
{
    if(sign)
    {
        QString data;
        data=serial->Data_Collect_nature();
        Fill_Data(data);
    }
    else
    {
        QMessageBox::warning(NULL,"警告","自然电位收集失败!");
    }
}
void nature::Data_Filled(bool sign)
{
    if(sign)
    {
        QString data;
        for(int i=0;i<20;i++)
        {
            data=serial->Data_Collect_nature();
            Fill_Data(data);
        }
    }
}
void nature::Fill_Data(QString str)
{
    if(!str.isEmpty())
    {
        ui->tableWidget->setItem(Row_data, Col_data, new QTableWidgetItem(str));
        if(Row_data==(ui->tableWidget->rowCount()-1))
        {
            if(Col_data==ui->tableWidget->columnCount())
            {
                new_col();
            }
            Row_data=0;
            Col_data+=1;
        }
        else
        {
            Row_data+=1;
        }
   }
}


