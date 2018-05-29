/*
Mediator server:-
1)receive message from client 
2)connect to special server
3)actas as mediator between server and client
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

struct client{
    int sfd;
    int p_num;
};

struct client* conn;

int port[3]={3261,3262,3263};
char ip[3][15]={"127.0.0.1","127.0.0.1","127.0.0.1"};

void go(void* s){
    conn = (struct client*)malloc(sizeof(struct client));
    conn = (struct client*)s;
    int newfd = (*conn).sfd;
    int num   = (*conn).p_num;
   
    int serverfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = htons(port[num-1]);
    inet_pton(AF_INET,ip[num-1],&servaddr.sin_addr);
    connect(serverfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    
    struct pollfd mediator[2];
    mediator[0].fd=newfd;
    mediator[1].fd=serverfd;
    
    mediator[0].events=POLLIN;
    mediator[1].events=POLLIN;
    
    while(1){
    int rv= poll(mediator,2,-1);
    if(rv>0){
        if(mediator[0].revents & POLLIN){
            char buf[100];
            int n = read(newfd,buf,100);
            write(serverfd,buf,n);
        }
        
        if(mediator[1].revents & POLLIN){
            char buf[100];
            int n = read(serverfd,buf,100);
            write(newfd,buf,n);
        }
    }
   }
}

int main(int argc,char *argv[]){
    int sockfd,newfd[3],i=0;
    pthread_t pt[3];
    
    struct sockaddr_in servaddr,cliaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);
    servaddr.sin_port=htons(3962);
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(sockfd,10);
    
    while(1){
        
        socklen_t len = sizeof(cliaddr);
        if((newfd[i] = accept(sockfd,(struct sockaddr*)&cliaddr,&len))<0)
            perror("Connection error");
        char buff[1];
        read(newfd[i],buff,1);
        int num = (int)(buff[0]-48);
        
            struct client cl;
            cl.sfd = newfd[i];
            cl.p_num  =num;
        pthread_create(&pt[i],NULL,(void*)&go,&cl);
        i++;
    }
    return 0;
}
        
