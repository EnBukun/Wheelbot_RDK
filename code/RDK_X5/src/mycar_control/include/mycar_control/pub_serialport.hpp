#ifndef PUB_SERIALPORT_HPP
#define PUB_SERIALPORT_HPP

#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <sys/types.h>
#include  <sys/signal.h>
#include  <sys/stat.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <limits.h>
#include  <string.h>


enum sp_dev_e
{
    COM0 = 0,
    COM1,
    COM2,
    COM3,
    ttyUSB0,
    ttyUSB1,
    ttyUSB2
};
class Serial
{
    public:
    Serial();
    ~Serial();

    int OpenPort(int index);
    int SetPara(int serialfd,int speed=2,int databits=8,int stopbits=1,int parity=0);


   int WriteData(int fd,const char *data,int datalength);

    void ClosePort(int fd);
    int BaudRate( int baudrate);
};


#endif // PUB_SERIALPORT_HPP