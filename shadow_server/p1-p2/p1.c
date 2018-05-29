#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

struct msg{
	long type;
	char buf[100];
};
int main(){
    struct msg mg;
    int n;
   
    int shmid = shmget(ftok("54687",123),4,IPC_CREAT | 0666);
    int* a = (int*)shmat(shmid,(void*)0,0);
    printf("%d\n",*a);
    printf("1)Message Queue 2)FIFO\n");
    scanf("\n%d",&n);
    
    if(n==1)
    {   
            kill(*a,SIGUSR1);
            int key = ftok("ppp46",2343);
	        int id = msgget(key,IPC_CREAT | 0666);
            int c =fork();
        if(c>0){
            while(1){
                mg.type = 10;
                printf("Enter data for p1\n");
                int n =read(0,mg.buf,100);
                msgsnd(msgid,&mg,sizeof(mg)-sizeof(long),0);
                    }
                }
        else
        {  
          while(1){
            msgrcv(msgid,&mg,sizeof(mg)-sizeof(long),2,0);
            printf("Message received from p2 is %s\n",mg.buf);
                    }
        }
        
    }
    else{
        kill(*a,SIGUSR2);
        int n = mkfifo("xyz",0777);
	    int fd = open("xyz",O_WRONLY);
	    int n2 = mkfifo("xyz123",0777);
	    int fd2 = open("xyz123",O_RDONLY);
	    int c =fork();
	    if(c>0)
	    {
	        while(1){
	        char buff[100];
	        int n3 = read(0,buff,100);
	        write(fd,buff,n3);
	        }
	    }
	    else{
	
        while(1){
                char buff[100];
               	int n4 =read(fd2,buff,100);
               	write(1,buff,n4);
           } 
        
        }
    return 0;
    }
    }

