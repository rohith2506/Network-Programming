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
	
	sockfd = socket(AF_INET, SOCK_STREAM,0);
	bzero (&servaddr , sizeof(servaddr));
	servaddr.sin_family = AF_INET ;
	servaddr.sin_port = htons(3962);
	inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
	
	if(connect(sockfd, (struct sockaddr  *) &servaddr , sizeof(servaddr))<0)
		perror("connection error");

		char buf[1];
	   	printf("enter ur requires service number\n");
		scanf("%c",&buf[0]);
		write(sockfd,buf,1);
		printf("now ur required server sent message\n");
		while(1){
		char buff[100];
		int len=read(sockfd,buff,100);
	        write(1,buff,len);
		}				

	return 0;
	}
	 

