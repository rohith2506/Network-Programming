#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
  #include <string.h>
  #include <sys/types.h>//for accept
  #include <sys/socket.h>//for accept
  #include <netinet/in.h>
      #include <arpa/inet.h>
      #include <netdb.h>
      #include <sys/stat.h>
      #include <pthread.h>
#include<fcntl.h>
      int sockfd;
void func(){
    
    int newfd= accept(sockfd,(struct sockaddr*)NULL,NULL);
    if(newfd<0)
    {
        perror("Client");
        exit(0);
    }
    while(1){
         char buf[100];
         mkfifo("Hello",0770);
         int fd = open("Hello",O_WRONLY);
         int n =read(newfd,buf,100);
       // write(0,buf,n);
         write(fd,buf,n);
    }
}
int main(){
    int i,j;
		sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htons(INADDR_ANY);
    i=1300;
    servaddr.sin_port=htons(i);
    printf("Port : %d\n",i);
    char yes='1';
   if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
           perror("setsockopt");
           exit(0);
   }
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    perror("Bind");
    listen(sockfd,30);
    
        for(i=0;i<5;i++){
        int c=fork();
        if(c==0){
            printf("%d\n",getpid());
            pthread_t t[10];

            for(j=0;j<5;j++){
                pthread_create(&t[j],NULL,(void*)&func,NULL);
            }
            for(j=0;j<5;j++)
                pthread_join(t[j],NULL);
            exit(0);
        }
        else{
        }
    }
    while(1){
                char buf[100];
                mkfifo("Hello",0770);
                int fd = open("Hello",O_RDONLY);
                int n = read(fd,buf,100);
                printf("Parent has read\n");
                write(1,buf,n);
    }
    return 0;
}