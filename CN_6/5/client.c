/*
This is client process requesting for special server through mediator
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
        perror("connection Error");
    int n;
    printf("Enter your required special number \n");
    scanf("%d",&n);
    char buf[1];
    buf[0]=n+48;
    write(sockfd,buf,1);
    
    while(1){
        char buff[100];
        printf("Receiving data from special server %d\n",n);
        int len =read(sockfd,buff,100);
        write(0,buff,len);
        printf("Sending some data to special server%d\n",n);
        len = read(0,buff,100);
        write(sockfd,buff,len);
    }
    return 0;
}


