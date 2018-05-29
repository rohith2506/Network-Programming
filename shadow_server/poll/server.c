#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
int main(int argc,char *argv[]){
	  printf("This is server\n");
	  struct pollfd a[3];
	  int fd[3];
	  mkfifo("client1",0777);
	  fd[0]=open("client1",O_RDONLY);
	  mkfifo("client2",0777);
	  fd[1]=open("client2",O_RDONLY);
	  mkfifo("client3",0777);
	  fd[2]=open("client3",O_RDONLY);
	  
	  char buf[100];
	  
	  a[0].fd=fd[0];
	  a[1].fd=fd[1];
	  a[2].fd=fd[2];
	  
	  a[0].events=POLLIN;
	  a[1].events=POLLIN;
	  a[2].events=POLLIN;
	  
	  //int r=poll(a,3,0);
	  
	  while(1){
		int r=poll(a,3,0);
		int i;
		if(r>0){
		for(i=0;i<3;i++){
			if(a[i].revents & POLLIN){
				printf("Data came from client %d\n",i);
				int n=read(fd[i],buf,100);
				write(1,buf,n);
			}
		}
	}
}
	return 0;
}
				
		
	  
