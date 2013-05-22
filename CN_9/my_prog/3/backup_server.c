#include<errno.h>
#include<stdio.h>
#include<sys/un.h>
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
#define MAX 3
                
  int recv_fd(int sfd)
{
	struct msghdr msg;
	/*do all the unwanted things first...
	same as the send_fd function..*/
	struct iovec io;
	char ptr[1];
	io.iov_base = ptr;
	io.iov_len = 1;
	msg.msg_name = 0;
	msg.msg_namelen = 0;
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	/*-----------------------*/
	
	
	char buf[CMSG_SPACE(sizeof(int))];
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);
	/*reasoning is same..as above*/
	
	/*now here comes the main part..*/
	
	if(recvmsg( sfd, &msg, 0)==-1)
	{
		/*some shit has happened*/
		perror("recv\n");
		exit(1);
	}
	
	struct cmsghdr *cm;
	
	cm =  CMSG_FIRSTHDR(&msg);
	/*get the first message header..*/
	
	if(cm->cmsg_type != SCM_RIGHTS)
	{
		/*again some shit has happened..*/
		perror("unknown type..\n");
		exit(1);
	}
	
	/*if control has reached here.. this means
	we have got the correct message..and when you 
	extract the fd out of this message 
	this need not be same as the one which was sent..
	allocating a new fd is all done by the kernel
	and our job is jst to use it..*/
	 printf("received fd:%d\n", *(int*)CMSG_DATA(cm));
	 return *(int*)CMSG_DATA(cm);
}	 	 		
int main(){
    int fd,size;
    struct sockaddr_un srvr;
   
    bzero(&srvr,sizeof(struct sockaddr_un));
    srvr.sun_family = AF_UNIX;
    strcpy(srvr.sun_path,"ff");
    
    if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
        perror("Socket:\n");
    
    size = sizeof(srvr.sun_family)+strlen(srvr.sun_path);

    if(connect(fd,(struct sockaddr *)&srvr,size)==-1)
        perror("connect:\n");
    
    int i,clifd[MAX];
    for(i=0;i<MAX;i++)
   	clifd[i]=recv_fd(fd);
   	for(i=0;i<MAX;i++){
   		if(fork()>0){
			while(1){
				char buf[100];
				int n =read(clifd[i],buf,100);
				write(1,buf,n);
			        }
		           }
			}
			while(1);
    return 0;
    }
    
   
 
