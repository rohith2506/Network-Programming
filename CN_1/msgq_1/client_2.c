#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
struct msg
{
	long type;
	char buf[10];
	int id;
};

int main(int argc,char *argv[])
{
	printf("Client_2\n");
	int key1=ftok("in111",999);
	int id2=msgget(key1,IPC_CREAT | 0666);
	struct msg mg;
	
	int pid=fork();
	
	if(pid>0)
	{
		while(1)
		{
		msgrcv(id2,&mg,sizeof(mg)-sizeof(long),20,0);
		printf("your message is %s \n",mg.buf);
		 }
	 }
	else
	{
		while(1)
		{
		scanf("%s",mg.buf);
		mg.type = 100;
		mg.id = 20;
		msgsnd(id2,&mg,sizeof(mg)-sizeof(long),0);
		
	}
}
	return 0;
}
		
	

