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
#include <pthread.h>
#define MAX 10

struct service{
int fd;
struct sockaddr_in cl;
};

void func1(void* fd){
     int* nfd = (int*)fd;
     int c =fork();
     if(c==0){
            dup2(*nfd,0);
            dup2(*nfd,1);
            execlp("./s1","s1",NULL);
            }
       }

void func2(void* fd){
     int* nfd = (int*)fd;
     int c =fork();
     if(c==0){
            dup2(nfd,0);
            dup2(nfd,1);
            execlp("./s2","s2",NULL);
            }
       }

void func3(void* st){
struct s *ser = (struct s*)st;
char buf[100];
FILE* fp = popen("./s3","r");
int fd1 = fileno(fp);
int n =read(fd1,buf,100);
sendto(ser->fd,buf,strlen(buf),0,(struct sockaddr*)&(ser->cl),sizeof(ser->cl));
}

int main(){
    int sfd[MAX],i=0;
    pthread_t p[MAX];
    for(i=0;i<5;i++)
        sfd[i] = socket(AF_INET,SOCK_STREAM,0);
    for(i=5;i<10;i++)
        sfd[i]=  socket(AF_INET,SOCK_DGRAM,0);
    struct sockaddr_in servaddr[10];
    for(i=0;i<10;i++){
    servaddr[i].sin_family=AF_INET;
    servaddr[i].sin_addr.s_addr=htons(INADDR_ANY);
    servaddr[i].sin_port=htons(3031+i);
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
    //printf("1\n");
    int nready= select(sfd[MAX-1]+1,&rfds,NULL,NULL,NULL);
    if(nready<0)
        perror("no readable descriptors");
        else{
           // printf("2\n");
            for(i=0;i<5;i++){
                if(FD_ISSET(sfd[i],&rfds)){
                    struct sockaddr_in cliaddr;
                    bzero(&cliaddr,sizeof(cliaddr));
                    int len = sizeof(cliaddr);
                    int nsfd = accept(sfd[i],(struct sockaddr*)&cliaddr,&len);
                    if(i==0){
                        printf("iam here\n");
                    pthread_create(&p[i],NULL,(void*)&func1,(void*)&nsfd);
                        }
                    if(i==1)
                    pthread_create(&p[i],NULL,(void*)&func2,(void*)&nsfd);
                }
            }

            for(i=5;i<10;i++){
               // printf("2\n");
            if(FD_ISSET(sfd[i],&rfds)){
              char buf[100];
              struct sockaddr_in cli;
              bzero(&cli,sizeof(&cli));
              int leng = sizeof(cli);
                int num = recvfrom(sfd[i],buf,100,0,(struct sockaddr*)&cli,&leng);
                struct s service;
                service.fd=sfd[i];
                service.cl=cli;
          		pthread_create(&p[i],NULL,(void*)&func3,(void*)&ser);
            }
         }
      }
    }
        return 0;
}

               




