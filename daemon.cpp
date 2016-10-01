#include "daemon.h"
#include <string.h>
#include <QDebug>
#include "usart.h"
//extern int linkfd;
void daemon::run(){
        int sockfd;
        sockaddr_nl sa;
        int len;
        char buf[4096];
        iovec iov;
        msghdr msg;

        int sig;
        memset(&sa,0,sizeof(sa));
        sa.nl_family=AF_NETLINK;
        sa.nl_groups=NETLINK_KOBJECT_UEVENT;
        sa.nl_pid = 0;//getpid(); both is ok
        memset(&msg,0,sizeof(msg));
        iov.iov_base=(void *)buf;
        iov.iov_len=sizeof(buf);
        msg.msg_name=(void *)&sa;
        msg.msg_namelen=sizeof(sa);
        msg.msg_iov=&iov;
        msg.msg_iovlen=1;

        sockfd=socket(AF_NETLINK,SOCK_RAW,NETLINK_KOBJECT_UEVENT);
        bind(sockfd,(struct sockaddr *)&sa,sizeof(sa));
        while(1){
            len=recvmsg(sockfd,&msg,0);
            if((sig=analysis(buf,len)))
                emit host(sig,adddev);
        }
}
int daemon::analysis(char *buf,int len){
    char *tmp=NULL;
    adddev.clear();
    if(strstr(buf,"add"))
        if((tmp=strstr(buf,"/tty/ttyUSB"))){
            adddev.append(tmp+5);
            return 1;
        }
    if(strstr(buf,"remove"))
        if((tmp=strstr(buf,"/tty/ttyUSB"))){
            adddev.append(tmp+5);
            return 2;
        }
    return 0;
}
void usart_recv::run(){
    QString recv_str;
    int n;
    while(1){
        if(linkfd>0){
            char buff[20];
            n=uart_recv(linkfd,buff,20);
            if(n>0){
                buff[n]='\0';
                recv_str.append(buff);
                emit recv(recv_str);
                recv_str.clear();
            }
        }
    }
}
