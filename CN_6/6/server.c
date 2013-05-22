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
#include <fcntl.h>
#include <sys/stat.h>

int m=3;
void *func(void *fd){
	int *fd1=(int *)fd;
	struct sockaddr_in servaddr,cliaddr;
	int sockfd =socket(AF_INET , SOCK_STREAM ,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port =htons(2345+m);
	
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	listen(sockfd,10);

	while(1){
		socklen_t len = sizeof(cliaddr);
		int newfd;
		if((newfd= accept(sockfd,(struct sockaddr *)&cliaddr,&len))<0)
			perror("connection error");
		while(1){
		char buf[100];
		int length=read(newfd,buf,100);
		write(*fd1,buf,length);	
		}
		}	
 }
	

int main(int argc,char *argv[]){
	pthread_t t[10];
	
	for(int i=0;i<10;i++){
		int c=fork();
		if(c==0){
		mkfifo("in1",0770);
                int fd=open("in1",O_WRONLY);
			for(int j=0;j<10;j++)
				pthread_create(&t[j],NULL,(void *)func,&fd);
			}
		else if(c>0){
			char buf[100];
			mkfifo("in1",0770);
			int fd1=open("in1",O_RDONLY);
			while(1){
			int len=read(fd1,buf,100);
			printf("message received by parent\n");
			write(1,buf,len);
			}
			}
		}
	return 0;
	}
