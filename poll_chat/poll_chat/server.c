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
	
	struct pollfd server[3];
	
	printf("This is server\n");
	char buf[100];
	int fd[3];
    mkfifo("client1",0777);
     fd[0]=open("client1",O_RDONLY);
    
    mkfifo("client2",0777);
     fd[1]=open("client2",O_WRONLY);
    
    
    server[0].fd=fd[0];
    server[0].events=POLLIN;
    
    server[1].fd=fd[1];
    server[1].events=POLLOUT;
    
    while(1){
		int r=poll(server,2,-1);
		int n;
		if(r>0){
			int i;
			for(i=0;i<2;i++){
				if(server[i].revents & POLLIN){
					 n=read(fd[i],buf,100);
				}
			    else if(server[i].revents & POLLOUT){
					 write(fd[i],buf,n);
				 }
				 else{}
			 }
		 }
	}
	
	return 0;
}	 
					
		
		
    
    
    
    
    
	
	
	
	
	
