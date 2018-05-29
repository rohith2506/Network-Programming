#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc,char *argv[]){
    int sfd[10],i=0;
    for(i=0;i<5;i++)
        sfd[i] = socket(AF_INET,SOCK_STREAM,0);
    for(i=5;i<10;i++)
        sfd[i]= socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in servaddr[10];
    for(i=0;i<10;i++){
    servaddr[i].sin_family=AF_INET;
    servaddr[i].sin_addr.s_addr=htons(INADDR_ANY);
    servaddr[i].sin_port=htons(2001+i);
    bind(sfd[i],(struct sockaddr*)&servaddr[i],sizeof(servaddr[i]));
    listen(sfd[i],10);
    }

    fd_set rfds;
    FD_ZERO(&rfds);
    for(i=0;i<10;i++)
        FD_SET(sfd[i],&rfds);
        while(1){
            FD_ZERO(&rfds);
            for(i=0;i<10;i++)
                FD_SET(sfd[i],&rfds);
                printf("1\n");
    int nready= select(sfd[9]+1,&rfds,NULL,NULL,NULL);
    printf("%d\n",nready);
    if(nready<0)
        perror("no readable descriptors");
        else{
            printf("2\n");
            for(i=0;i<5;i++){
                if(FD_ISSET(sfd[i],&rfds)){
                    struct sockaddr_in cliaddr;
                    bzero(&cliaddr,sizeof(cliaddr));
                    int len = sizeof(cliaddr);
                    int nsfd = accept(sfd[i],(struct sockaddr*)&cliaddr,&len);
                    if(fork()){
                        dup2(nsfd,1);
                        dup2(nsfd,0);
                        if(i==0)
                            execlp("./s1","s1",NULL);
                        else if(i==1)
                            execlp("./s2","s2",NULL);
                        }
                }
            }

            for(i=5;i<10;i++){
                printf("iam here too\n");
            if(FD_ISSET(sfd[i],&rfds)){
              struct sockaddr_in cli;
              bzero(&cli,sizeof(&cli));
              int leng = sizeof(cli);
              char buf[100];
              int num = recvfrom(sfd[i],buf,100,0,(struct sockaddr*)&cli,&leng);
              printf("%d\n",num);
              FILE* fp = popen("./s3","r");
              int fd = fileno(fp);
              int n =read(fd,buf,100);
                sendto(sfd[i],buf,n,0,(struct sockaddr*)&cli,sizeof(cli));
             
          }
        }
     }
  }
        return 0;
}

               




