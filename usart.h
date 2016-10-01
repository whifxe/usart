#ifndef USART_H
#define USART_H
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <QDebug>
extern int linkfd;
extern int config[5];
extern int gbps[];
extern int bit[];
inline int uart_send(int fd, char *data, int datalen)
{
    int len = 0;
    len = write(fd, data, datalen);
    if(len == datalen) {
        return len;
    } else {
        tcflush(fd, TCOFLUSH);
        return -1;
    }
    return 0;
}
inline int uart_recv(int fd, char *data, int datalen)
{
    int len=0;
    fd_set fs_read;
    struct timeval tv_timeout;
    FD_ZERO(&fs_read);
    FD_SET(fd, &fs_read);
    tv_timeout.tv_sec  = (10*20/115200+2);
    tv_timeout.tv_usec = 0;
    select(fd+1, &fs_read, NULL, NULL, &tv_timeout);
    if (FD_ISSET(fd, &fs_read)) {
        len=read(fd,data,datalen);
        return len;
    }else{
        return -1;
    }
    return 0;
}
inline int init_usart(char *dev,int ctrl[5])
{
    int serial_fd = open(dev,O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial_fd < 0)
        return -1;
    struct termios options;
    tcgetattr(serial_fd, &options);
    options.c_cflag |= (CLOCAL | CREAD);//设置控制模式状态，本地连接，接收使能
    options.c_cflag &= ~CSIZE;//字符长度，设置数据位之前一定要屏掉这个位
    options.c_cflag &= ~CRTSCTS;//无硬件流控
    cfsetospeed(&options,ctrl[0]);//设置波特率
    options.c_cflag |=ctrl[1];//8位数据长度
    switch (ctrl[2])
    {
       case 0:
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
       case 1:
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;
            break;
       case 2:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;
            break;
       case 3:
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
              return -1;
    }
    //options.c_iflag |= ctrl[2];//无奇偶检验位
    options.c_cflag &= ~CSTOPB;//1位停止位
    options.c_oflag = 0; //输出模式
    options.c_lflag = 0; //不激活终端模式
    tcflush(serial_fd, TCIFLUSH);//溢出数据可以接收，但不读
    tcsetattr(serial_fd, TCSANOW, &options);
    return serial_fd;
}
#endif
