#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	char buf[100];
	mkfifo("client2",0777);
	int fd=open("client2",O_WRONLY);
	
		while(1){
	int n=read(0,buf,100);
	write(fd,buf,n);
}
    return 0;
}
