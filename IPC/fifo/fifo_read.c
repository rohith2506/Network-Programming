#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char *argv[]){
        char buf[100];
        int fd=open("in1",O_RDONLY);
        int n;
	printf("Read file\n");
	if((n=read(fd,buf,10))>0)
	  write(1,buf,n);
	  write(1,"\n",1);
	 return 0;
}
	 
