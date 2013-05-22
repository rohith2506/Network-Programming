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
    int sockfd;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(2367);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    while(1){
        char buf[100];int num=0;
        printf("Enter some message for server\n");
        int n=read(0,buf,100);
        sendto(sockfd,buf,n,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
            int len = sizeof(servaddr);
            num=recvfrom(sockfd,buf,100,0,(struct sockaddr*)&servaddr,&len);
            write(1,buf,strlen(buf));
            }    
return 0;
}
