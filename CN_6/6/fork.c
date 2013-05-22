#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc,char *argv[]){
	int fd[2];
	pid_t pid;
	pipe(fd);	
	char buf[100];
	int j=0;
	while(j<3){
		int c=fork();
		if(c==0){
			close(fd[0]);
			pid = getpid();
			write(fd[1],(int *)pid,10);
			}
		else if(c>0){
			close(fd[1]);
			int len=read(fd[0],buf,10);
			write(1,buf,len);
			}
		else{}
		j++;
		}	

	return 0;
}	
