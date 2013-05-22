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

#define MAXLINE 100

int main(int argc,char *argv[]){
	int i,maxi,maxfd,listenfd,connfd,sockfd;
	int nready,client[FD_SETSIZE];
	
	ssize_t n;
	fd_set rset,allset;
	char buf[MAXLINE];
	
	socklen_t clilen;
	struct sockaddr_in cliaddr , servaddr;
	
	listenfd = socket(AF_INET , SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr  = htonl(INADDR_ANY);
	servaddr.sin_port  = htons(3962);

	bind(listenfd , (struct sockaddr *)&servaddr , sizeof(servaddr));

	listen(listenfd , 10);

	maxfd = listenfd;
	printf("iam here\n");
	for(i=0;i<FD_SETSIZE; i++)
		client[i]=-1;
	
	FD_ZERO(&allset);
	FD_SET(listenfd , &allset);

	while(1){
		rset = allset;
		nready = select(maxfd+1,&rset , NULL , NULL , NULL);
		
		if(FD_ISSET(listenfd , &rset)){
			clilen = sizeof(cliaddr);
			connfd = accept(listenfd , (struct sockaddr *)&cliaddr , &clilen);

			for(i=0; i<FD_SETSIZE;i++){
				if(client[i] < 0){
					client[i] = connfd;
					break;
					}
				}
			if(i == FD_SETSIZE)
				perror("too many clients");
			
			FD_SET(connfd , &allset);
			if(connfd > maxfd)
				maxfd = connfd;
			if(i > maxi)
				maxi = i;
			if(--nready <= 0)
				continue;
			}
		
		for(i=0;i<=maxi;i++){
			if((sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd , &rset)){
				printf("iam here twoooo\n");
				int p=fork();
				if(p==0){
					//close all sfds except sockfd
					for( i=0;i<=maxi;i++){
						if(sockfd != client[i])
							close(client[i]);
							}
					close(listenfd);
					
					int n = read(sockfd , buf ,MAXLINE);
					
					if(n == 0){
						close(sockfd);
						FD_CLR(sockfd , &allset);
						client[i] = -1;
						}
					int num = (int)(buf[0]-48);
					printf("%d\n",num);
					
					dup2(sockfd , 0);
					dup2(sockfd , 1);
	
					if(num == 1)
						execlp("./s1","s1",NULL);
					else if(num == 2)
						execlp("./s2","s2",NULL);
					else if(num == 3)
						execlp("./s3","s3",NULL);
					else if(num == 4)
						execlp("./s4","s4",NULL);
					else{}
					}
				else
					close(sockfd);
	
				}		
				
			if(--nready <= 0)
				break;
			}
		}

	return 0;
}

	
		
		










				






	


	
