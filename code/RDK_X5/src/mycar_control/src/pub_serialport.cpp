#include "pub_serialport.hpp"
#include  <termios.h>

Serial::Serial()
{

}
Serial::~Serial()
{

}

int Serial::BaudRate( int baudrate)
{
    if(7 == baudrate)
        return B460800;
    else if(6 == baudrate)
        return B115200;
    else if(5 == baudrate)
        return B57600;
    else if(4 == baudrate)
        return B38400;
    else if(3 == baudrate)
        return B19200;
    else if(2 == baudrate)
        return B9600;
    else if(1 == baudrate)
        return B4800;
    else if(0 == baudrate)
        return B2400;
    else
        return B9600;

}

int Serial::SetPara(int serialfd,int speed,int databits , int stopbits ,int parity )
{
    struct termios termios_new;
    bzero( &termios_new, sizeof(termios_new));//等价于memset(&termios_new,sizeof(termios_new));
    cfmakeraw(&termios_new);//就是将终端设置为原始模式
    termios_new.c_cflag=BaudRate(speed);
    termios_new.c_cflag |= CLOCAL | CREAD;
  //  termios_new.c_iflag = IGNPAR | IGNBRK;

    termios_new.c_cflag &= ~CSIZE;
    switch (databits)
    {
    case 0:
        termios_new.c_cflag |= CS5;
        break;
    case 1:
        termios_new.c_cflag |= CS6;
        break;
    case 2:
        termios_new.c_cflag |= CS7;
        break;
    case 3:
        termios_new.c_cflag |= CS8;
        break;
    default:
        termios_new.c_cflag |= CS8;
        break;
    }

    switch (parity)
    {
    case 0:                  //as no parity
        termios_new.c_cflag &= ~PARENB;    //Clear parity enable
      //  termios_new.c_iflag &= ~INPCK; /* Enable parity checking */  //add by fu
        break;
    case 1:
        termios_new.c_cflag |= PARENB;     // Enable parity
        termios_new.c_cflag &= ~PARODD;
        break;
    case 2:
        termios_new.c_cflag |= PARENB;
        termios_new.c_cflag |= ~PARODD;
        break;
    default:
        termios_new.c_cflag &= ~PARENB;   // Clear parity enable
        break;
    }
    switch (stopbits)// set Stop Bit
    {
    case 1:
        termios_new.c_cflag &= ~CSTOPB;
        break;
    case 2:
        termios_new.c_cflag |= CSTOPB;
        break;
    default:
        termios_new.c_cflag &= ~CSTOPB;
        break;
    }
    tcflush(serialfd,TCIFLUSH); // 清除输入缓存
   tcflush(serialfd,TCOFLUSH); // 清除输出缓存
    termios_new.c_cc[VTIME] = 1;   // MIN与 TIME组合有以下四种：1.MIN = 0 , TIME =0  有READ立即回传 否则传回 0 ,不读取任何字元
    termios_new.c_cc[VMIN] = 1;  //    2、 MIN = 0 , TIME >0  READ 传回读到的字元,或在十分之一秒后传回TIME 若来不及读到任何字元,则传回0
    tcflush (serialfd, TCIFLUSH);  //    3、 MIN > 0 , TIME =0  READ 会等待,直到MIN字元可读
    return tcsetattr(serialfd,TCSANOW,&termios_new);  //    4、 MIN > 0 , TIME > 0 每一格字元之间计时器即会被启动 READ 会在读到MIN字元,传回值或
}

int Serial::WriteData(int fd,const char *data, int datalength )//index 代表串口号 0 串口/dev/ttyAMA1 ......
{
    if(fd <0){ return -1;}
    int len = 0, total_len = 0;//modify8.
    for (total_len = 0 ; total_len < datalength;)
    {
        len = 0;
        len = write(fd, &data[total_len], datalength - total_len);
        printf("WriteData fd = %d ,len =%d,data = %s\n",fd,len,data);
        if (len > 0)
        {
            total_len += len;
        }
        else if(len <= 0)
        {
            len = -1;
            break;
        }
     }
       return len;
}

int Serial::ReadData(int fd,unsigned char *data, int datalength)
{
       if(fd <0){ return -1;}
       int len = 0;
       memset(data,0,datalength);

       int max_fd = 0;
       fd_set readset ={0};
       struct timeval tv ={0};

       FD_ZERO(&readset);
       FD_SET((unsigned int)fd, &readset);
       max_fd = fd +1;
       tv.tv_sec=0;
       tv.tv_usec=1000;
       if (select(max_fd, &readset, NULL, NULL,&tv ) < 0)
       {
               printf("ReadData: select error\n");
       }
       int nRet = FD_ISSET(fd, &readset);
      if (nRet)
       {
              len = read(fd, data, datalength);
       }
       return len;
}

void Serial::ClosePort(int fd)
{
    struct termios termios_old;
    if(fd > 0)
    {
        tcsetattr (fd, TCSADRAIN, &termios_old);
        ::close (fd);
    }
}

int  Serial::OpenPort(int index)
{
    char *device;
    struct termios termios_old;
    int fd;
    switch(index)
    {
    case COM0:  device="/dev/ttyS0";  break;
    case COM1:  device="/dev/ttyS1";  break;
    case COM2:  device="/dev/ttyS2";  break;
  //  case COM3:  device="/dev/ttyS3";  break;
    case COM3:  device="/dev/pts/2";  break;
    case ttyUSB0:  device="/dev/ttyUSB0";  break;
    case ttyUSB1:  device="/dev/ttyUSB1";  break;
    case ttyUSB2:  device="/dev/ttyUSB2";  break;
    default: device="/dev/ttyAMA2"; break;
    }

    fd = open( device, O_RDWR | O_NOCTTY |O_NONBLOCK);//O_RDWR | O_NOCTTY | O_NDELAY   //O_NONBLOCK
    if (fd < 0)
    { return -1;}
    tcgetattr(fd , &termios_old);
    return fd;
}
