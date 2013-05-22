#include<stdio.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>    
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>
struct mesg{
long type;
char buf[100];
};

void func(){
    execlp("./rams4serv" , "rams4serv",NULL,0);
    }
int main()
{   
   signal(SIGINT,func);
    struct mesg m;
    int msgid = msgget(ftok("rams4cl1.c",1),IPC_CREAT |0777);
    int c =fork();
    if(c>0)
    {   
        m.type=2;
        read(0,m.buf,10);
        msgsnd(msgid,&m,sizeof(struct mesg)-sizeof(long),0);
    }
    else
    {   
        while(1)
        {
            
            msgrcv(msgid,&m,sizeof(struct mesg)-sizeof(long),1,0);
            
            printf("Received from Client is  %s\n",m.buf);
        }
    }
    return 0;
}    
