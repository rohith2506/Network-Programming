#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <sys/select.h>
#include <pthread.h>
#define MAX 10
int sfd[MAX];
struct service{
	int fd;
	struct sockaddr_in cl;
	};


void func1(void *fd){
	int *nsfd = (int *)fd;
	int c=fork();
	if(c==0){
		dup2(*nsfd,0);
		dup2(*nsfd,1);
		execlp("./s1","s1",NULL);
		}
	}

void func2(void *fd){
	int *nsfd = (int *)fd;
	int c = fork();
	if(c == 0){
		dup2(*nsfd,0);
		dup2(*nsfd,1);
		execlp("./s2","s2",NULL);
		}
	}

void func3(void *st){
	struct service *ser = (struct service *)st;
	char buf[100];
	FILE *fp = popen("./s3","r");
	int fd1 = fileno(fp);
	int n = read(fd1,buf,100);
	sendto(ser->fd,buf,strlen(buf),0,(struct sockaddr *)&(ser->cl),sizeof(ser->cl));
	}


void *func(){
	int i;
	pthread_t p[10];
	fd_set rfds;
	for(i=0;i<10;i++)
		FD_SET(sfd[i],&rfds);
		while(1){
			FD_ZERO(&rfds);
			for(i=0;i<10;i++)
				FD_SET(sfd[i],&rfds);
			int nready = select(sfd[MAX-1]+1,&rfds,NULL,NULL,NULL);
			if(nready < 0)
				perror("No readable descriptors");
			else{
				for(i=0;i<5;i++){
					if(FD_ISSET(sfd[i],&rfds)){
						struct sockaddr_in cliaddr;
						bzero(&cliaddr,sizeof(cliaddr));
						socklen_t len = sizeof(cliaddr);
						int nsfd = accept(sfd[i],(struct sockaddr *)&cliaddr,&len);
						if(i==0)
						  pthread_create(&p[i],NULL,(void *)&func1,(void *)&nsfd);
						if(i==1)
						  pthread_create(&p[i],NULL,(void *)&func2,(void *)&nsfd);
						}
					}
				
				for(i=5;i<10;i++){
					if(FD_ISSET(sfd[i],&rfds)){
					       struct sockaddr_in cli;
					       bzero(&cli,sizeof(cli));
					       socklen_t len = sizeof(cli);
					       int num;
					       char buff[100];
					       if(num = recvfrom(sfd[i],buff,100,0,(struct sockaddr *)&cli,&len)==-1)
							perror("receiving error");
					       printf("%s\n",buff);
					       struct service ser;
					       ser.fd = sfd[i];
					       ser.cl = cli;
					       pthread_create(&p[i],NULL,(void *)&func3,(void *)&ser);
						}
					} 
			  	 }
		  	  }
 		}			   					  
	 


int main(int argc,char *argv[]){
	pthread_t t[10];
	int i,j;
	for(i=0;i<5;i++)
		sfd[i] = socket(AF_INET , SOCK_STREAM , 0);
	for(i=5;i<10;i++)
		sfd[i] = socket(AF_INET , SOCK_DGRAM , 0);
	struct sockaddr_in servaddr[10];

	for(i=0;i<5;i++){
		perror("hi:");
		servaddr[i].sin_family = AF_INET;
		servaddr[i].sin_port = htons(3001+i);
		 char yes='1';
		if(setsockopt(sfd[i],SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
           		perror("setsockopt");
           		exit(0);
        		 }
		servaddr[i].sin_addr.s_addr = htons(INADDR_ANY);
		bind(sfd[i],(struct sockaddr*)&servaddr[i],sizeof(servaddr[i]));
		perror("bind:");
		listen(sfd[i],10);
		}

	for(i=5;i<10;i++){
		perror("hi:");
		servaddr[i].sin_family = AF_INET;
		servaddr[i].sin_port = htons(3031+i);
		 char yes='1';
		if(setsockopt(sfd[i],SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1){
           		perror("setsockopt");
           		exit(0);
        		 }
		servaddr[i].sin_addr.s_addr = htons(INADDR_ANY);
		bind(sfd[i],(struct sockaddr*)&servaddr[i],sizeof(servaddr[i]));
		perror("bind:");
		//listen(sfd[i],10);
		}

	for(i=0;i<10;i++){
		int c=fork();
		if(c==0){
			for(j=0;j<10;j++)
				pthread_create(&t[j],NULL,(void *)func,NULL);
			
			for(j=0;j<10;j++)
				pthread_join(t[j],NULL);
			}

		}
	while(1);
	return 0;
	}	



		
