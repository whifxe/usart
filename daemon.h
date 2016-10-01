#ifndef DAEMON_H
#define DAEMON_H

#include <QThread>
#include <sys/types.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>
class daemon : public QThread
{
    Q_OBJECT
public:
    void run();
signals:
    void host(int,QString);
private:
    int analysis(char *buf, int len);
    QString adddev;
};
class usart_recv : public QThread
{
    Q_OBJECT
public:
    void run()__attribute__((optimize("-O0")));;
signals:
    void recv(QString);
};
#endif // DAEMON_H
