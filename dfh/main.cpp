#include"color_local.hpp"
#include"uart.hpp"
#include<sys/types.h>
#include <stdlib.h>
#include<unistd.h>
using namespace std;

int main()
 {
    int tty_fd;
    tty_fd=Uart_OpenDev(TTY_DEV);
    usart_config(tty_fd);
    pid_t pid;

    pid = fork();
    if (-1==pid)
    {
     cout<<"error to create  new process!"<<endl;
     return 0;
    }
  
  else if (pid==0)
  {
    color_local(); 
      
  }
  else 
  {
      while(1)
      {
         Uart_SendFrame(tty_fd); 
      }
  }
  
  return 0;
 }