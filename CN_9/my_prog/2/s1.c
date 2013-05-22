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

int s2_pid,fd2,newfd;
 void pass_fd(int sfd, int fd_to_send)
{
	struct msghdr msg;
	
	char buf[CMSG_SPACE(sizeof(int))];
	
	struct cmsghdr *cmsg;
	
	struct iovec ve;	
	
	char *st ="I";
	
	ve.iov_base = st;
	ve.iov_len =1;
	
	
	msg.msg_iov = &ve;
	msg.msg_iovlen = 1;
	

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	
	

	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);

	cmsg = CMSG_FIRSTHDR(&msg);

  	cmsg->cmsg_level = SOL_SOCKET;
  	cmsg->cmsg_type = SCM_RIGHTS;
  	cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_send));

	
	*(int*)CMSG_DATA(cmsg) = fd_to_send;

	msg.msg_controllen = cmsg->cmsg_len;
	
	
	msg.msg_flags = 0;

	
	if(sendmsg( sfd, &msg, 0)==-1){ perror("snd:\n"); exit(1); }

	printf("sent fd:%d\n", fd_to_send);

}
void func(){
    signal(SIGUSR1,func);
      char buf[10];
             int n =read(fd2,buf,10);
	     printf("10 characters from server1:\n");
             if(n==0)
		exit(0);
               write(1,buf,n);
               kill(s2_pid,SIGUSR1);
              }

int main(){
    int fd,size;
    struct sockaddr_un srvr;
    
   signal(SIGUSR1,func);
    bzero(&srvr,sizeof(struct sockaddr_un));
    srvr.sun_family = AF_UNIX;
    strcpy(srvr.sun_path,"ff");
    
    if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
        perror("Socket:\n");
    
    unlink(srvr.sun_path);
    size = sizeof(srvr.sun_family)+strlen(srvr.sun_path);

    bind(fd,(struct sockaddr*)&srvr,size);
    perror("Bind:\n");
    listen(fd,5);
    socklen_t len = sizeof(srvr);
    newfd  = accept(fd,(struct sockaddr*)&srvr,&len);
    perror("Accept:\n");
     FILE* fp = fopen("in.txt","r");
     perror("fopen:\n");

     fd2 = fileno(fp);
     pass_fd(newfd,fd2);
     
     int pid = getpid();
     char pth[10];
     int num = sprintf(pth,"%d",pid);
     write(newfd,pth,num);
     int n = read(newfd,pth,10);
     s2_pid = atoi(pth);
	
     kill(s2_pid,SIGUSR1);
		while(1);
    return 0;
    }
    
   
 
