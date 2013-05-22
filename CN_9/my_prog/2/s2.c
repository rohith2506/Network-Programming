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
                
  int recv_fd(int sfd)
{
	struct msghdr msg;
	struct iovec io;
	char ptr[1];
	io.iov_base = ptr;
	io.iov_len = 1;
	msg.msg_name = 0;
	msg.msg_namelen = 0;
	msg.msg_iov = &io;
	msg.msg_iovlen = 1;
	
	
	char buf[CMSG_SPACE(sizeof(int))];
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);
	printf("iam herr in recv\n");
	if(recvmsg( sfd, &msg, 0)==-1)
	{
		perror("recv\n");
		exit(1);
	}
	
	struct cmsghdr *cm;
	
	cm =  CMSG_FIRSTHDR(&msg);
	
	if(cm->cmsg_type != SCM_RIGHTS)
	{
		perror("unknown type..\n");
		exit(1);
	}
	 printf("received fd:%d\n", *(int*)CMSG_DATA(cm));
	 return *(int*)CMSG_DATA(cm);
}	 	 
int s1_pid,fd2;	

void func(){
signal(SIGUSR1,func);
    char buf[10];
    printf("10 characters from server2:\n");
    int  n =read(fd2,buf,10);
    perror("read\n");
     if(n==0)
         exit(0);
     write(1,buf,n);
  kill(s1_pid,SIGUSR1);
}


int main(){
    int fd,size;
    struct sockaddr_un srvr;
    signal(SIGUSR1,func); 
    bzero(&srvr,sizeof(struct sockaddr_un));
    srvr.sun_family = AF_UNIX;
    
    if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
        perror("Socket:\n");
   strcpy(srvr.sun_path,"ff");
    size = sizeof(srvr.sun_family)+strlen(srvr.sun_path);
    if(connect(fd,(struct sockaddr *)&srvr,size)==-1)
        perror("connect:\n");
    
   fd2 = recv_fd(fd);
   char buf[10];
   
		int n = read(fd,buf,10);
		s1_pid = atoi(buf);
		int num = sprintf(buf,"%d",getpid());
		write(fd,buf,num);
   while(1);
      return 0;
    }
    
   
 
