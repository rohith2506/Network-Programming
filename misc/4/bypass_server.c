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
int port[3]={3261,3262,3263};
char ip[3][15]={"127.0.0.1","127.0.0.1","127.0.0.1"};

int main(){
    int sockfd,newfd,i=0;
    
    struct sockaddr_in servaddr,cliaddr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);
    servaddr.sin_port=htons(3962);
   
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    listen(sockfd,10);
    
    while(1){
        bzero(&cliaddr,sizeof(cliaddr));
        socklen_t len = sizeof(cliaddr);
        if((newfd = accept(sockfd,(struct sockaddr*)&cliaddr,&len))<0)
			printf("connection error\n");
        
        char buff[1];
        int n;
        read(newfd,buff,1);
        n = (int)(buff[0]-48);
        printf("requesting process number %d\n",n);
        char *client_ip = inet_ntoa(cliaddr.sin_addr);
        char client_port[5];
        int num = read(newfd,client_port,4);
        client_port[4]='\0';
        
        //creating new request for group 2 with process 1 ip and port number
        
        int groupfd_1 = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in servaddr;
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_port = htons(port[n-1]);
        inet_pton(AF_INET,ip[n-1],&servaddr.sin_addr);
        printf("%s\t%s\t%s\n",ip[n-1],client_ip,client_port);
        
        if(connect(groupfd_1,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
			printf("connection error\n");
      
        write(groupfd_1,client_ip,strlen(client_ip));
        /*char ch[1];
        read(groupfd_1,ch,1);*/
        write(groupfd_1,client_port,strlen(client_port));
          }
    return 0;
}
        
