#include<sys/socket.h>
#include<errno.h>
#include<stdio.h>
#include<sys/un.h>
#include<sys/types.h>
int main(){
    int fd,size;
    struct sockaddr_un local;
   
    bzero(&local,sizeof(struct sockaddr_un));
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path,"ff");
    
    if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
        perror("Socket:\n");
    
    size = sizeof(local.sun_family)+strlen(local.sun_path);

    if(connect(fd,(struct sockaddr *)&local,size)==-1)
        perror("connect:\n");
    
    while(1){
        char buf[100];
        printf("Enter data\n");
        int num = read(0,buf,100);
        write(fd,buf,num);
        }

    return 0;
}
