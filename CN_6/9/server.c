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
    
    struct sockaddr_in servaddr,cliaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
   
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr); 
    servaddr.sin_port=htons(3962);
    
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(sockfd,10);
    

        newfd = accept(sockfd,(struct sockaddr*)NULL,NULL);
        char buff[2];
        int n = read(newfd,buff,2);
        if(buff[0]=='1'){
            close(sockfd);
            sockfd = socket(AF_INET,SOCK_DGRAM,0);
            bzero(&servaddr,sizeof(servaddr));
            servaddr.sin_family=AF_INET;
            inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
            servaddr.sin_port=htons(3962);

            bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        
        while(1){
        bzero(&cliaddr,sizeof(cliaddr));
        socklen_t len = sizeof(cliaddr);
        char buf[100];
        int num=0;
        if((num=recvfrom(sockfd,buf,100,0,(struct sockaddr*)&cliaddr,&len))==-1)
            perror("receiving error");
        else{
            buf[num]='\0';
            printf("received data from client via udp connection\n");
            write(1,buf,num);
            printf("\n");
            printf("enter some data to client\n");
            int n = read(0,buf,100);
            if((num=sendto(sockfd,buf,n,0,(struct sockaddr*)&cliaddr,sizeof(cliaddr)))==-1)
                perror("sending error");
            }
        
            }
           }
           else{
            while(1){
            char buf[100];
            int n  = read(newfd,buf,100);
            printf("received data from client via tcp connection \n");
            write(1,buf,n);
            printf("Enter some message for client \n");
            n = read(0,buf,100);
            write(newfd,buf,n);
           }
        }

    return 0;
}
        
