/*
group 2:-
1)receives request from bypass server
2)send message to group2 process
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
    int sockfd,newfd,i=0;
    
    struct sockaddr_in servaddr,cliaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);
    servaddr.sin_port=htons(3261);
    
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(sockfd,10);
    
        bzero(&cliaddr,sizeof(cliaddr));
        socklen_t len = sizeof(cliaddr);
        if((newfd = accept(sockfd,(struct sockaddr*)&cliaddr,&len))<0)
			printf("connection error\n");
        
        char ip[20],pport[5];int n,num=0;
        
        n = read(newfd,ip,20);
        n=read(newfd,pport,5);
        int port = atoi(pport);
        
        int fd1 = socket(AF_INET,SOCK_STREAM,0);
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_port = htons(port);
        inet_pton(AF_INET,ip,&servaddr.sin_addr);
        
        if(connect(fd1,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
            perror("connectioon error");
            
        while(1){
            char buff[100];
            printf("Send message to group1 process\n");
            int n =read(0,buff,100);
            write(fd1,buff,n);
            n = read(fd1,buff,100);
            write(1,buff,n);
                }
    return 0;
}
        

