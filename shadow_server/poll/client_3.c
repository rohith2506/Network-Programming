#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc,char *argv[]){
	char buf[100];
	mkfifo("client3",0777);
	int fd=open("client3",O_WRONLY);
	
	while(1){
	int n=read(0,buf,100);
	write(fd,buf,n);
}
    return 0;
}
