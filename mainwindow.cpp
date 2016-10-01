#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include "usart.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),link(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GetConfigure(){
    dev=ui->cdev->currentText();
    bps=ui->cbps->currentIndex();
    databit=ui->cdatabit->currentIndex();
    checkbit=ui->ccheckbit->currentIndex();
    stopbit=ui->cstopbit->currentIndex();
    flowctrl=ui->cflowctrl->currentIndex();
    config[0]=gbps[bps];
    config[1]=bit[databit];
    config[2]=checkbit;
}
void MainWindow::ShowConfigure(){
    ui->rxascii->click();
    ui->txascii->click();
    ui->cbps->setCurrentIndex(3);
    ui->cdatabit->setCurrentIndex(2);
    ui->cdev->addItems(GetDev());
    QLabel *version=new QLabel(this);
    version->setText("version 1.0.0");
    ui->statusBar->addPermanentWidget(version);
    linkstatus=new QLabel(this);
    if(!(ui->cdev->count()))
        linkstatus->setText("No Device!");
    else
        linkstatus->setText("No Link!");
    ui->statusBar->addWidget(linkstatus);
    QLabel *space=new QLabel(this);
    space->setText("      ");
    ui->statusBar->addWidget(space);
    QLabel *rxspeed=new QLabel(this);
    rxspeed->setText("RX:0");
    ui->statusBar->addWidget(rxspeed);
    QLabel *space1=new QLabel(this);
    space1->setText("      ");
    ui->statusBar->addWidget(space1);
    QLabel *txspeed=new QLabel(this);
    txspeed->setText("TX:0");
    ui->statusBar->addWidget(txspeed);
}

void MainWindow::on_cbps_currentIndexChanged(const QString &arg1)
{
    if(arg1=="custom"){
        ui->cbps->setEditable(true);
        ui->cbps->clearEditText();
        return;
    }
    ui->cbps->setEditable(false);
}

QStringList MainWindow::GetDev(){
    QStringList ret;
    DIR *dir;
    struct dirent *ptr;
    dir=opendir("/dev");
    while((ptr=readdir(dir))!=NULL){
        if(!strncmp(ptr->d_name,"ttyUSB",6))
            ret<<ptr->d_name;
    }
    return ret;
}

void MainWindow::on_action_L_triggered()
{
    QIcon imageL;
    if(!link){
        GetConfigure();
        if(!dev.isEmpty()){
            QString path("/dev/");
            path+=dev;
            QByteArray tmp=path.toLatin1();
            char *devname=tmp.data();
            linkfd=init_usart(devname,config);
            if(linkfd==-1)return;
        }
        linkstatus->setText((dev+"  link"));
        imageL.addFile(QString::fromUtf8(":/image/image/stop.png"));
        ui->action_L->setIcon(imageL);
        link=true;
    }else{
        if(linkfd){
            ::close(linkfd);
            linkfd=0;
        }
        linkstatus->setText("No Link!");
        imageL.addFile(QString::fromUtf8(":/image/image/link.png"));
        ui->action_L->setIcon(imageL);
        link=false;
    }
}
void MainWindow::dev_host(int val,QString addname){
    int n=ui->cdev->count();
    if(val==1){
        ui->cdev->insertItem(n,addname);
        if(!n)linkstatus->setText("No Link!");
    }
    else{
        ui->cdev->removeItem(ui->cdev->findText(addname));
        if(!(--n))linkstatus->setText("No Device!");
    }
}

void MainWindow::on_send_clicked()
{
    QString send=ui->lineEdit->text();
    QByteArray tmp=send.toLatin1();
    char *data=tmp.data();
    uart_send(linkfd,data,send.length());
}
void MainWindow::addtext(QString add){
    ui->textEdit->append(add);
}
