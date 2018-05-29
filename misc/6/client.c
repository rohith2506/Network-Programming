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
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET , SOCK_STREAM , 0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(2348);
	
	if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
		perror("connection error");
	
	while(1){
		char buff[100];
		printf("ready to send some message to prefork server\n");
		printf("enter some message\n");
		int len = read(0,buff,100);
		write(sockfd,buff,len);
		}
	return 0;
	}
	
