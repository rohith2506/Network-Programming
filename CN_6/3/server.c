#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>

int main(int argc,char *argv[]){
	int listenfd;
	socklen_t len;
	struct sockaddr_in servaddr;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero (&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET ;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(3692);
	
	bind(listenfd , (struct sockaddr *) &servaddr, sizeof(servaddr) );
	
	listen(listenfd , 10);
	
	while(1){
		int connfd = accept(listenfd , (struct sockaddr *) NULL ,  NULL);
		printf("connection success\n");
		if(fork()==0){
		        close(listenfd);
		        char buff[1];
				read(connfd,buff,1);
		        int i=(int)(buff[0]-48);
				printf("%d\n",i);
			dup2(connfd,0);
			dup2(connfd,1);
			if(i==1)
				execlp("./s1","s1",NULL);
			else if(i==2)
				execlp("./s2","s2",NULL);
			else if(i==3)
				execlp("./s3","s3",NULL);
			else
			 	execlp("./s4","s4",NULL);
				
			}
		}
	return 0;
	}
					 
	         
