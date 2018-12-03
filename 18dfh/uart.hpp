#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#define buff_size  512//发送数组的大小,默认512.
#define TTY_DEV  "/dev/ttyUSB0"// 串口设备,默认为tty0. 
extern char Recv_Buf[buff_size];
int Uart_OpenDev(char *dev);
void Uart_SendFrame_L(int fd);
void Uart_SendFrame_R(int fd);
void Uart_RecvFrame(int fd);
void usart_config(int fd);
