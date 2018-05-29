#include<stdio.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<signal.h>    
#include<sys/shm.h>
#include<sys/ipc.h>
struct mesg{
long type;
char buf[100];
};

int main()
{   
    
    struct mesg m;
    int msgid = msgget(ftok("rams4cl1.c",1),IPC_CREAT |0777);
    int c =fork();
    if(c>0)
    {   
        m.type=1;
        while(1){
        read(0,m.buf,10);
        msgsnd(msgid,&m,sizeof(struct mesg)-sizeof(long),0);
    }
    }
    else
    {
        while(1)
        {
            msgrcv(msgid,&m,sizeof(struct mesg)-sizeof(long),2,0);
            printf("Received from server %s\n",m.buf);
        }
    }
    return 0;
}    
