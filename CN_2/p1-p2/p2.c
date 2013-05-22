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

void func(){
    signal(SIGUSR1,func);
    struct msg mg;
    int key = ftok("9484",3);
    int msgid = msgget(key,IPC_CREAT | 0666);
	int c =fork();
    if(c>0)
        { 
            while(1){
             mg.type = 2;
            printf("Enter data for p2\n");
            int n =read(0,mg.buf,100);
            msgsnd(msgid,&mg,sizeof(mg)-sizeof(long),0);
            }
         }
        else
        {   while(1){
            msgrcv(msgid,&mg,sizeof(mg)-sizeof(long),1,0);
            printf("Message received from p1 is %s\n",mg.buf);
            }
        }
}

void func2(){
    signal(SIGUSR2,func2);
    int n = mkfifo("xyz",0777);
	int fd = open("xyz",O_RDONLY);
	int n2 = mkfifo("xyz123",0777);
	int fd2 = open("xyz123",O_WRONLY);
    int c =fork();
	if(c>0)
	 {
	    while(1){
	    char buf[100];
	    int n3 = read(0,buf,100);
	    write(fd2,buf,n3);
	            }
	 }
	 else{
	
        while(1){
            char buf[100];
           	int n4 =read(fd,buf,100);
           	write(1,buf,n4);
           	}
        }   
}

int main(){
	signal(SIGUSR1,func);
	signal(SIGUSR2,func2);
   
    int shmid = shmget(ftok("54687",123),4,IPC_CREAT | 0666);
    int* a = (int*)shmat(shmid,(void*)0,0);
    a[0]=getpid();
    while(1);
    return 0;
}   
            
