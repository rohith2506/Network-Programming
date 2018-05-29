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
    struct sockaddr_in servaddr,clientaddr;
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(3962);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    
    bzero(&clientaddr,sizeof(clientaddr));
    clientaddr.sin_family=AF_INET;
    clientaddr.sin_port = htons(3001);
    inet_pton(AF_INET,"127.0.0.1",&clientaddr.sin_addr);
    int fd= socket(AF_INET,SOCK_DGRAM,0);
    bind(fd,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
    
    struct pollfd sercli[2];
    sercli[0].fd=0;
    sercli[0].events=POLLIN;
    sercli[1].fd=fd;
    sercli[1].events=POLLIN;
    while(1){

        int num=0,i;
        int ret = poll(sercli,2,-1);
        if(ret>0){
            for(i=0;i<2;i++){
                char buff[100];
                if(sercli[i].revents & POLLIN){
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
