#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <poll.h>

int main(int argc,char* argv[]){
        int sockfd = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in serv;
        serv.sin_family = AF_INET;
        inet_pton(AF_INET,"127.0.0.1",&serv.sin_addr);
        serv.sin_port = htons(3001);
         int num;
         if((num=connect(sockfd,(struct sockaddr*)&serv,sizeof(serv)))==-1)
            perror("error");
  
            struct pollfd server_client[2];
            server_client[0].fd=0;
            server_client[0].events=POLLIN;
            server_client[1].fd=sockfd;
            server_client[1].events=POLLIN;
            int i=0;

         while(1){
             char buf[100];
             int FD = poll(server_client,2,-1);
             if(FD>0){
                 for(i=0;i<2;i++){
                     if(server_client[i].revents & POLLIN){
                        if(i==0){
                            int n = read(0,buf,100);
                            write(sockfd,buf,n);
                        }
                        else{
                            int n = read(sockfd,buf,100);
                            write(1,buf,n);
                        }
                     }
                 }
             }
   }
   return 0;
   }


