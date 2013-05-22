/*
group 1:-
1)sends client request to bypass server
2)receives messages from group2 process
*/
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
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(3962);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
        perror("Connection error");
    int n;
    printf("Enter group2 process u want to connect \n");
    scanf("%d",&n);
    char buf[1];
    buf[0]=n+48;
    write(sockfd,buf,1);
    char port_num[4]={'3','6','6','6'};
    write(sockfd,port_num,4);

//sending request to bypass server completed

    char buffer[100];
    servaddr.sin_port = htons(3666);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int groupfd_2;
    groupfd_2 = socket(AF_INET,SOCK_STREAM,0);
    bind(groupfd_2,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(groupfd_2,10);
    int newfd ;
    if((newfd = accept(groupfd_2,(struct sockaddr*)NULL,NULL))<0)
        perror("Connection error");
    while(1){
        char buf[100];
        int len = read(newfd,buf,100);
        printf("message received from group 2\n");
        write(1,buf,len);
        printf("send message to group 2 process\n");
        len = read(0,buf,100);
        write(newfd,buf,len);
           }
return 0;
}

			
				
							
	
