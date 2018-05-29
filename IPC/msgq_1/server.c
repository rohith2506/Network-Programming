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
int client[3];
int n=3;
int main(int argc,char *argv[])
{
	printf("main chat server\n");
	client[0]=10;
	client[1]=20;
	client[2]=30;
	int key1=ftok("in111",999);
	int id1=msgget(key1,IPC_CREAT | 0666);
	struct msg mg;
	int i;
	
while(1){
msgrcv(id1,&mg,sizeof(mg)-sizeof(long),100,0);
for(i=0;i<n;i++)
{
if(mg.id != client[i])
{
mg.type=client[i];
msgsnd(id1,&mg,sizeof(mg)-sizeof(long),0);
}
}
}
	return 0;
}
		
	


