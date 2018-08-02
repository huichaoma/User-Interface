#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
    per1 = new QLabel("欢迎使用岩土电参数测量仪 ！");
    energy=new QLabel();
    energy->setText("电池电量:");
    statusBar()->addPermanentWidget(energy);
    statusBar()->addWidget(per1);

    QMenu *configurationMenu =menuBar()->addMenu(QObject::tr("基本测量功能  "));
    QAction *powerAction       = new QAction(QObject::tr("电源频率参数  "),parent);
    QAction *potentialAction       = new QAction(QObject::tr("自然电位测量  "),parent);
    QAction *resistanceAction      = new QAction(QObject::tr("接地电阻测量  "),parent);
    QAction *four_electrodesAction = new QAction(QObject::tr("电阻率极测量"),parent);
    configurationMenu->addAction(powerAction);
    configurationMenu->addAction(potentialAction);
    configurationMenu->addAction( resistanceAction);
    configurationMenu->addAction( four_electrodesAction);
    connect(powerAction,&QAction::triggered,this,&MainWindow::powerAction);
    connect(potentialAction,&QAction::triggered,this,&MainWindow::potentialAction);
    connect(resistanceAction,&QAction::triggered,this,&MainWindow::resistanceAction);
    connect(four_electrodesAction,&QAction::triggered,this,&MainWindow::four_electrodesAction);

    QMenu *shutMenu=menuBar()->addMenu(QObject::tr("开关控制"));
    QAction *Reboot_MCU= new QAction(QObject::tr("重  启MCU"),parent);
    QAction *Reboot_pi= new QAction(QObject::tr("重    启"),parent);
    QAction *Shut_pi= new QAction(QObject::tr("关    闭"),parent);
    shutMenu->addAction(Reboot_MCU);
    shutMenu->addAction(Reboot_pi);
    shutMenu->addAction(Shut_pi);
    connect(Reboot_MCU,&QAction::triggered,this,&MainWindow::Reboot_MCU);
    connect(Reboot_pi,&QAction::triggered,this,&MainWindow::Reboot_pi);
    connect(Shut_pi,&QAction::triggered,this,&MainWindow::Shut_pi);

    QMenu *helpMenu   = menuBar()->addMenu(QObject::tr("帮助    "));
    QAction *quit= new QAction(QObject::tr("开发选项"),parent);
    QAction *network=new QAction(QObject::tr("网络检测"),parent);
    QAction *aboutAction= new QAction(QObject::tr("关于    "),parent);
    helpMenu->addAction(quit);
    helpMenu->addAction(network);
    helpMenu->addAction(aboutAction);
    connect(quit,&QAction::triggered,this,&MainWindow::quit);
    connect(network,&QAction::triggered,this,&MainWindow::network);
    connect(aboutAction,&QAction::triggered,this,&MainWindow::aboutAction);

    pow=new QProgressBar();
    pow->setOrientation(Qt::Horizontal);  // 水平方向
    pow->setFormat("%p%");
    pow->setRange(0,100);
    statusBar()->addPermanentWidget(pow);
    //Threads for sending power collection commands on time

    connect(new_thread,SIGNAL(Power_Signal(int)),this,SLOT(Receive_Power(int)));
    connect(new_thread,SIGNAL(Com_Signal(bool)),this,SLOT(Receive_Com(bool)));
    connect(new_thread,SIGNAL(Command_Block_Emit(bool)),this,SLOT(Receive_Com_Block(bool)));
    //new_thread->start();
    serial->Execute_Command_Pointer=serial->Stor_Command_Pointer;
    Power_Interface=new power();
    Nature_Interface=new nature();
    Resistance_Interface=new resistance();
    Four_Interface=new four_electrodes();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete pow;
    delete per1;
    delete Power_Interface;
    delete Nature_Interface;
    delete Resistance_Interface;
    delete Four_Interface;
    delete energy;
}
void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(NULL,"提示","请先收集数据");
}
void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::information(NULL,"提示","请先收集数据");
}
void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(NULL,"提示","请先收集数据");
}
void MainWindow::on_pushButton_5_clicked()
{
    QMessageBox::information(NULL,"提示","请先收集数据");
}
void MainWindow::powerAction()
{
    if(serial->Serial_Flag<0)
        QMessageBox::warning(NULL,"警告","串口没有打开");
    else
        Power_Interface->show();
}
void MainWindow::potentialAction()
{
    if(serial->Serial_Flag<0)
        QMessageBox::warning(NULL,"警告","串口没有打开");
    else
    {
        Nature_Interface->show();
        serial->Interface_Signal="Nat";
    }
}
void MainWindow::resistanceAction()
{
    if(serial->Serial_Flag<0)
    {
        QMessageBox::warning(NULL,"警告","串口没有打开");
    }
    else
    {
        if(serial->Power_fre)
        {
            Resistance_Interface->show();
            serial->Interface_Signal="Res";
        }
        else
            QMessageBox::warning(NULL,"警告","请先配置电源频率");
    }
}
void MainWindow::four_electrodesAction()
{
    if(serial->Serial_Flag<0)
        QMessageBox::warning(NULL,"警告","串口没有打开");
    else
    {
        if(serial->Power_fre)
        {
            Four_Interface->show();
            serial->Interface_Signal="Fou";
        }
        else
            QMessageBox::warning(NULL,"警告","请先配置电源频率");
    }
}
void MainWindow::Reboot_MCU()
{
   serial->Send_Command("01CO1234ST\0",0,1,300);
}
void MainWindow::Reboot_pi()
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(NULL,tr("重启界面"),QString(tr("确认重启界面？")),QMessageBox::Yes | QMessageBox::No);
    if(button==QMessageBox::Yes){
        system("reboot");
    }
}
void MainWindow::Shut_pi()
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(NULL,tr("关闭界面"),QString(tr("确认关闭界面？")),QMessageBox::Yes | QMessageBox::No);
    if(button==QMessageBox::Yes){
        system("poweroff");
    }
}
void MainWindow::aboutAction()
{
    Dialog *about=new Dialog();
    about->show();
}
void MainWindow::network()
{

}
void MainWindow::quit()
{
    exploit *S=new exploit();
    connect(S,SIGNAL(sendData(QString)),this,SLOT(receiveData(QString)));
    S->show();
}
void MainWindow::receiveData(QString data)
{
    if(data=="ok")
    {
        serial->close_serial();
        this->close();
    }
}
void MainWindow::Receive_Power(int value)
{
    pow->setValue(value);
    if(value<=20)
    {
        pow->setStyleSheet("QProgressBar::chunk { background-color: rgb(255,0, 0) }");
        QMessageBox::warning(NULL,"警告","电量不足，请及时备份数据");
    }
    else
        pow->setStyleSheet("QProgressBar::chunk { background-color: rgb(0, 255, 0) }");
}
void MainWindow::Receive_Com(bool sign)
{
    if(sign)
    {
        for(int j=0;j<10;j++)
        {
            if(!QString::compare(serial->Execute_Command_Pointer->Com_Data,QStringLiteral("RE")))
            {
                serial->Command_Recognition(serial->Execute_Command_Pointer);
                serial->Clear_Buffer(serial->Execute_Command_Pointer,1);
                serial->Execute_Command_Pointer=serial->Execute_Command_Pointer->Next;
                break;
            }
            serial->Execute_Command_Pointer=serial->Execute_Command_Pointer->Next;
        }
    }
}
void MainWindow::Receive_Com_Block(bool sign)
{
    if(sign)
    {
        serial->Clear_Buffer(serial->Execute_Command_Pointer,10);
        QMessageBox::warning(NULL,"警告","请重新配置");
    }
}

