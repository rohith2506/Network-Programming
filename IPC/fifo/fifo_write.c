#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
   char buf[100];
   int i=mkfifo("in1",O_CREAT | 0666);
   if(i<0)
     perror("Fifo error");
   printf("Write file\n");
   int fd=open("in1",O_WRONLY);
   int n=read(0,buf,10);
   write(fd,buf,n);
   return 0;
   }