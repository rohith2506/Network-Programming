#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/un.h>

void pass_fd(int sfd, int fd_to_send)
{
	struct msghdr msg;
	
	/*allocate memory to 'msg_control' field in msghdr struct */
	char buf[CMSG_SPACE(sizeof(int))];
	/*the memory to be allocated should include data + header..
	this is calculated by the above macro...(it merely adds some
	no. of bytes and returs that number..*/
	
	struct cmsghdr *cmsg;
	
	struct iovec ve;	
	/*must send/receive atleast one byte...
	main purpose is to have some error 
	checking.. but this is completely 
	irrelevant in the current context..*/
	
	char *st ="I";
	/*jst let us allocate 1 byte for formality 
	and leave it that way...*/
	ve.iov_base = st;
	ve.iov_len =1;
	
	/*attach this memory to our main msghdr struct...*/
	msg.msg_iov = &ve;
	msg.msg_iovlen = 1;
	
	/*these are optional fields ..
	leave these fields with zeros..
	to prevent unnecessary SIGSEGVs..*/
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	
	
	/*here starts the main part..*/
	/*attach the 'buf' to msg_control..
	and fill in the size field correspondingly..
	*/
	
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);
	
	/*actually msg_control field must 
	point to a struct of type 'cmsghdr'
	we just allocated the memory, yet we need to 
	set all the corresponding fields..
	It is done as follows:
	*/
	cmsg = CMSG_FIRSTHDR(&msg);
	/* this macro returns the address in the buffer..
	from where the first header starts..
	*/
	
	/*set all the fields appropriately..*/
  	cmsg->cmsg_level = SOL_SOCKET;
  	cmsg->cmsg_type = SCM_RIGHTS;
  	cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_send));
	/*in the above field we need to store
	the size of header + data(in this case 4 bytes(int) for our fd..
	this is returned by the 'CMSG_LEN' macro..*/
	
	*(int*)CMSG_DATA(cmsg) = fd_to_send;
	/*after the above three fields we keep the actual data..
	the macro 'CMSG_DATA' returns pointer to this location
	and we set it to the file descriptor to be sent..
	*/
	
	msg.msg_controllen = cmsg->cmsg_len;
	/*now that we have filled the 'cmsg' struct 
	we store the size of this struct..*/
	/*this one isn't required when you
	pass a single fd..
	but useful when u pass multiple fds.*/
	
	msg.msg_flags = 0;
	/*leave the flags field zeroed..*/
	
	if(sendmsg( sfd, &msg, 0)==-1){ perror("snd:\n"); exit(1); }
	/*send this over the UNIX deomain socoket..*/ 
	printf("sent fd:%d\n", fd_to_send);
	close(fd_to_send);
	/*close the fd which was sent..*/
}

int main(int argc,char *argv[]){
	struct sockaddr_un sa,cli;
	struct sockaddr_in s,c;
	struct msghdr msg;
	int i,j,ufd,sfd,fd;
	ufd=socket(AF_UNIX,SOCK_STREAM,0);
	sa.sun_family=AF_UNIX;	
	char b[]="ff";
	unlink(b);
	strncpy(sa.sun_path,b,sizeof(sa.sun_path)-1);
	bind(ufd,(struct sockaddr*)&sa,sizeof(sa));
	perror("unix bind");
	sfd=socket(AF_INET,SOCK_STREAM,0);
	s.sin_family=AF_INET;
	s.sin_addr.s_addr=htonl(INADDR_ANY);
	s.sin_port=htons(2367);
	bind(sfd,(struct sockaddr*)&s,sizeof(s));
	perror("tcp bind");
	listen(sfd,5);
	socklen_t t;
	t=sizeof(t);
	fd=accept(sfd,(struct sockaddr*)&c,&t);
	perror("accept");
	printf("accepted in s1\n");
	char buff[100];
	i=recv(fd,buff,100,0);
	write(1,buff,i);
	printf("s2 starts\n");
	t=sizeof(sa);
	connect(ufd,(struct sockaddr*)&sa,sizeof(sa));
	perror("connect");
	bzero(&msg,sizeof(msg));
	printf("fd is:%d\n",fd);
	pass_fd(ufd,fd);
	close(fd);
	return 0;
}
