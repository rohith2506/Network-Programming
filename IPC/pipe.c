#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define BUFF 4096

int main(int argc,char *argv[]){
  int fd[2];
  char buf[BUFF];
  int n;
  if(pipe(fd)<0){
       printf("Error in creating pipe\n");
       exit(2);
         }
  else{
        int pid;
        pid=fork();
        if(pid<0){
         printf("Error in creating fork\n");
         exit(0);
          }
       else if(pid>0){
	   n=read(0,buf,10);
           close(fd[0]);
           write(fd[1],buf,n);
           close(fd[1]);
           wait(NULL);
          }			
      else{
          close(fd[1]);
          if((n=read(fd[0],buf,BUFF))>0)
 	       write(1,buf,n);
               write(1,"\n",1);
          close(fd[0]);
          exit(0);
        }
   }
 
  return 0;
}


