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
	int grpid;
};

int main()
{
	printf("client_1\n");
	int key1=ftok("in111",999);
	int id1=msgget(key1,IPC_CREAT | 0666);
	struct msg mg;
	int pid=fork();
	
	if(pid>0)
	{
		while(1)
		{
		msgrcv(id1,&mg,sizeof(mg)-sizeof(long),10,0);
		printf("your message is %s\n",mg.buf);
		}
	 }
	else
	{
		while(1)
		{
		scanf("%s",mg.buf);
		mg.type = 100;
		mg.id=10;
		printf("enter the group for which you want to message\n");
		scanf("%d",&mg.grpid);
		msgsnd(id1,&mg,sizeof(mg)-sizeof(long),0);
		
	}
}
	return 0;
}
		
	


