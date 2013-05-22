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

int main(){
    int sockfd,newfd,i=0;
    
    struct sockaddr_in servaddr,clientaddr;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);
    servaddr.sin_port=htons(2367);
    
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(sockfd,5);
    
    while(1){
        bzero(&clientaddr,sizeof(clientaddr));
        socklen_t len = sizeof(clientaddr);
        char buf[100];int num=0;
        if((num=recvfrom(sockfd,buf,100,0,(struct sockaddr*)&clientaddr,&len))==-1)
            perror("recvfrom");
        else{
            write(1,buf,strlen(buf));
            printf("Send some message to client..\n");
            int n = read(0,buf,100);
            if((num=sendto(sockfd,buf,n,0,(struct sockaddr*)&clientaddr,sizeof(clientaddr)))==-1)
                perror("sendto");
            }
    }
    return 0;
}
        
