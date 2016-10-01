#include "mainwindow.h"
#include "daemon.h"
#include <QObject>
#include <QtGui>
//#include <dbus/dbus.h>
volatile int linkfd=0;
int config[5];
int gbps[]={B1200,B2400,B4800,B9600,B19200};
int bit[]={CS5,CS7,CS8};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    daemon t;
    usart_recv r;
    QObject::connect(&t,SIGNAL(host(int,QString)),&w,SLOT(dev_host(int,QString)));
    QObject::connect(&r,SIGNAL(recv(QString)),&w,SLOT(addtext(QString)));
    t.start();
    r.start();
    w.ShowConfigure();
    w.show();
    return a.exec();
}
