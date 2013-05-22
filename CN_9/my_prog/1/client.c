#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

int main(){
	struct sockaddr_in sa;
	int sfd,i,j;
	bzero(&sa,sizeof(sa));
	sfd=socket(AF_INET,SOCK_STREAM,0);
	sa.sin_addr.s_addr=htonl(INADDR_ANY);
	sa.sin_family=AF_INET;
	sa.sin_port=htons(2367);
	connect(sfd,(struct sockaddr*)&sa,sizeof(sa));
	perror("connect");
	char buff[100];
	while(1){
		i=read(0,buff,100);
		send(sfd,buff,i,0);
		perror("send");
	}
	return 0;
}
