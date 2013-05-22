#include <stdio.h>
#include <errno.h>
#include <sys/un.h>
#include<signal.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
 
int main(){
    int fd,size;
    struct sockaddr_un srvr;
    

    bzero(&srvr,sizeof(struct sockaddr_un));
    srvr.sun_family = AF_UNIX;
    strcpy(srvr.sun_path,"f1");
    
    if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
        perror("Socket:\n");
    
    unlink(srvr.sun_path);
    size = sizeof(srvr.sun_family)+strlen(srvr.sun_path);

    bind(fd,(struct sockaddr*)&srvr,size);
    perror("Bind:\n");
    listen(fd,5);
    socklen_t len = sizeof(srvr);
    
    int cnt=0;
    int pid,num;
    char temp_buff[10];
    while(1){
    int newfd  = accept(fd,(struct sockaddr*)&srvr,&len);
    perror("Accept:\n");
    if(cnt>0){ 
	    //printf("iam here\n");
            write(newfd,temp_buff,num); 
            break;
    	     }
    else{
	num = read(newfd,temp_buff,10);
        pid = atoi(temp_buff);
	//printf("pid \n",pid);
	cnt++;
    if(fork()>0){
    	while(1){
    		char buf[100];
		//printf("cnt is %d\n",cnt);
    		printf("Send message for client\n");
    		int n =read(0,buf,100);
    		write(newfd,buf,n);
   		}
    	    }
   	 }
    }
     while(1);
     return 0;
}
    
    
  	
