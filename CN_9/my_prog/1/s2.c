#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/un.h>

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
	int sfd,fd,i,j;
	struct msghdr msg;
	struct sockaddr_un sa,cli;
	sfd=socket(AF_UNIX,SOCK_STREAM,0);
	bzero(&sa,sizeof(sa));
	sa.sun_family=AF_UNIX;
	char path[]="ff";
	unlink(path);
	strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
	bind(sfd,(struct sockaddr*)&sa,sizeof(sa));
	perror("bind");
	listen(sfd,5);
	socklen_t t;
	t=sizeof(cli);
	fd=accept(sfd,(struct sockaddr*)&cli,&t);
	perror("accept");
	i=recv_fd(fd);
	printf("accepted fd:%d\n",i);
	char buff[100];
	printf("inside s2\n");
	while(1){
		j=recv(i,buff,100,0);
		write(1,buff,j);
	}
return 0;
}
