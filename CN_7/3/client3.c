#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>

int main(){
    int sockfd,newfd,i=0;
    
    struct sockaddr_in servaddr,cliaddr;
    bzero(&servaddr,sizeof(servaddr));
    
    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    servaddr.sin_port=htons(3036);
    
            sockfd = socket(AF_INET,SOCK_DGRAM,0);
            bzero(&cliaddr,sizeof(cliaddr));
            cliaddr.sin_family=AF_INET;
            cliaddr.sin_port = htons(3333);
            bind(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
        
        while(1){
        char buf[100];
        int num=0;
        int n = read(0,buf,100);
        if((num=sendto(sockfd,buf,n,0,(struct sockaddr*)&servaddr,sizeof(servaddr)))==-1)
                perror("sending error");
            
        else{
        socklen_t len = sizeof(servaddr);
        if((num=recvfrom(sockfd,buf,100,0,(struct sockaddr*)&servaddr,&len))==-1)
            perror("receiving error");
            buf[num]='\0'; 
            printf("data recevied from server \n");
            write(1,buf,num);
            printf("\n");
            }
        }
                        
    return 0;
}
        
