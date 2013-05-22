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
int main(int argc,char* argv[]){
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in servaddr,cliaddr;
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(3962);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    
    bzero(&cliaddr,sizeof(cliaddr));
    cliaddr.sin_family=AF_INET;
    cliaddr.sin_port = htons(3002);
    inet_pton(AF_INET,"127.0.0.1",&cliaddr.sin_addr);
    int fd= socket(AF_INET,SOCK_DGRAM,0);
    bind(fd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
    
    struct pollfd server_client[2];
    server_client[0].fd=0;
    server_client[0].events=POLLIN;
    server_client[1].fd=fd;
    server_client[1].events=POLLIN;
    while(1){

        int num=0,i;
        int ret = poll(server_client,2,-1);
        if(ret>0){
            for(i=0;i<2;i++){
                char buff[100];
                if(server_client[i].revents & POLLIN){
                   if(i==0){
                     int n =read(0,buff,100);
                     if(n!=0){
                        if((num=sendto(fd,buff,n,0,(struct sockaddr*)&servaddr,sizeof(servaddr)))==-1)
                            perror("sending error");
                       }
                   }
                   else{
                            int len = sizeof(servaddr);
                            
                            if((num=recvfrom(fd,buff,100,0,(struct sockaddr*)&servaddr,&len))==-1)
                                perror("receiving error");
                                write(1,buff,num);
                                printf("\n");
                        }    
                    }
                }
        }
    }
    return 0;
}
