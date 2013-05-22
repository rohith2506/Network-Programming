/*
this is special server which is connected to mediator 
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
        if( (newfd = accept(sockfd,(struct sockaddr*)&cliaddr,&len))<0)
			perror("connection error");
       
        while(1){
            printf("send some message  to client\n");
            char buff[100];
            int n =read(0,buff,100);
            write(newfd,buff,n);
            n = read(newfd,buff,100);
            printf("receiving some message from client:\n");
            write(1,buff,n);
                }
                
    return 0;
}
        
