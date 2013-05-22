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
#define MAX 100
int main(){
    int sockfd,newfd,i=0,count=0;
    
    struct sockaddr_in servaddr,cliaddr[MAX],cliaddrs;
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(3962);
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    for(i=0;i<MAX;i++)
        cliaddr[i].sin_port=0;
    
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    while(1){
        bzero(&cliaddrs,sizeof(cliaddrs));

        socklen_t len = sizeof(cliaddrs);
        char buff[100];
        int num=0;
        if((num=recvfrom(sockfd,buff,100,0,(struct sockaddr*)&cliaddrs,&len))==-1)
            perror("receiving error");
        else{
            int flag=0;
            for(i=0;i<count;i++){
                if(cliaddrs.sin_port==cliaddr[i].sin_port){
                    flag=1;
                    break;
                }
            }
             if(flag==1){
                 for(i=0;i<count;i++){
                     if(cliaddrs.sin_port!=cliaddr[i].sin_port){
                     socklen_t l=sizeof(cliaddr[i]);
                     if((num=sendto(sockfd,buff,num,0,(struct sockaddr*)&cliaddr[i],l))==-1)
                        perror("sending error");
                     }
                }
             }

            else{    
                bzero(&cliaddr[count],sizeof(cliaddr[count]));
                cliaddr[count].sin_family=AF_INET;
                cliaddr[count].sin_port = cliaddrs.sin_port;

                inet_pton(AF_INET,inet_ntoa(cliaddrs.sin_addr),&cliaddr[count].sin_addr);
                count++;
                }
        }
    }
    return 0;
}
        
