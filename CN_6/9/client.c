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
    
    struct sockaddr_in servaddr,clientaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    
    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    servaddr.sin_port=htons(3962);   
    connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Enter your choice  0)connection oriented 1)Connectionless oriented\n");
    char buf[2];
    int n = read(0,buf,2);
    write(sockfd,buf,1);
    if(buf[0]=='1'){
            close(sockfd);
            sockfd = socket(AF_INET,SOCK_DGRAM,0);
            bzero(&clientaddr,sizeof(clientaddr));
            clientaddr.sin_family=AF_INET;
            clientaddr.sin_port = htons(3001);
            bind(sockfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr));
        
        while(1){
        char buf[100];
        int num=0;
        printf("send some data to server\n");
        n = read(0,buf,100);
        if((num=sendto(sockfd,buf,n,0,(struct sockaddr*)&servaddr,sizeof(servaddr)))==-1)
                perror("sendingerror");
            
        else{
        socklen_t len = sizeof(servaddr);
        if((num=recvfrom(sockfd,buf,100,0,(struct sockaddr*)&servaddr,&len))==-1)
            perror("receiving error");
            buf[num]='\0'; 
            printf("receving data from server \n");
            write(1,buf,num);
            printf("\n");
            }
        }
    }
                        
        else{
            while(1){
            char buf[100];
            printf("Enter some messgae for server\n");
            int n  = read(0,buf,100);
            write(sockfd,buf,n);
            n = read(sockfd,buf,100);
            printf("server has sent some message\n");
            write(1,buf,n);
                    
           }
        }

    return 0;
}
        
