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
	/*close the fd which was sent..*/
}
              
  	 
int fd,fd2;	 
char buf[100]; 	
struct sockaddr_un cli;	

void func(){
	socklen_t len = sizeof(cli);
	int newfd  = accept(fd2,(struct sockaddr*)&cli,&len);
	printf("to give permission press Y else N:\n");
    	char ch[1];
    	int num =read(0,ch,1);
    	write(newfd,ch,num);
    	if(ch[0]=='Y'){
    	pass_fd(newfd,fd);
    	exit(0);
    	}
}
    	

int main(){
     int size;
     signal(SIGUSR1,func);
     struct sockaddr_un srvr;
 
    bzero(&srvr,sizeof(struct sockaddr_un));
    srvr.sun_family = AF_UNIX;
    strcpy(srvr.sun_path,"f1");
    
    if((fd=socket(AF_UNIX,SOCK_STREAM,0))<0)
        perror("Socket:\n");
    
    size = sizeof(srvr.sun_family)+strlen(srvr.sun_path);

    if(connect(fd,(struct sockaddr *)&srvr,size)==-1)
        perror("connect:\n");

    char buff[10];
    int n = sprintf(buff,"%d",getpid());
    write(fd,buff,n);
    
    bzero(&cli,sizeof(struct sockaddr_un));
    cli.sun_family = AF_UNIX;
    strcpy(cli.sun_path,"c1");
    
    if((fd2=socket(AF_UNIX,SOCK_STREAM,0))<0)
        perror("Socket:\n");
    
    unlink(cli.sun_path);
    size = sizeof(cli.sun_family)+strlen(cli.sun_path);

    bind(fd2,(struct sockaddr*)&cli,size);
    perror("Bind:\n");
    listen(fd2,5);
    
    while(1){
    int n = read(fd,buf,100);
    write(1,buf,n);
    }
    return 0;
    }
    
   

