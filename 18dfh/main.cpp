#include "color.hpp"
#include"uart.hpp"
#include <stdlib.h>
#include "opencv2/highgui.hpp"
using namespace std;
 int fd;
int main()
 {
  char flag=0; 
  fd = Uart_OpenDev(TTY_DEV);
  usart_config(fd);
  while(1)
  {
    Uart_RecvFrame(fd);
   flag=atoi(Recv_Buf);
   if (flag==111)
      {
       color1(); 
      }
    else if(flag==222)
    {
     color2();
    } 
    printf("%d",flag);   
  }   
  
  return 0;
 }
