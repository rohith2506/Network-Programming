#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <string.h>
#include <poll.h>

int main(int argc,char *argv[]){
	struct pollfd client[2];
	int fd[2];
	mkfifo("client1",0666);
	fd[0]=open("client1",O_WRONLY);
	
	mkfifo("client2",0666);
	fd[1]=open("client2",O_RDONLY);
	
	client[0].fd=fd[0];
	client[0].events=POLLOUT;
	
	client[1].fd=fd[1];
	client[1].events=POLLIN;
	  char buf[100];
     int n;
	while(1){
		int n;
		int r=poll(client,2,0);
		if(r>0){
			int i;
			for(i=0;i<2;i++){	 
			    if(client[i].revents & POLLOUT)
					n=read(0,buf,100);
			    else if(client[i].revents & POLLIN)
			       write(fd[i],buf,n);
	            else{}
			}
		}
	}
	return 0;
}
	
