#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>
struct msg
{
	long type;
	char buf[10];
	int id;
	int grpid;
	
};
int client[3][2];
int n=3;
int main(int argc,char *argv[])
{
	printf("This is main chat server\n");
	client[0][0]=10;
	client[0][1]=1;
	client[1][0]=20;
	client[1][1]=1;
	client[2][0]=30;
	client[2][1]=2;
	
	int key1=ftok("in111",999);
	int id1=msgget(key1,IPC_CREAT | 0666);
	struct msg mg;
	int i;
	
while(1){
msgrcv(id1,&mg,sizeof(mg)-sizeof(long),100,0);
int cnt=0;
for(i=0;i<n;i++)
if(client[i][1]==mg.grpid)
cnt++;
if(cnt==0)
{
	mg.type=mg.id;
	char buff[10]="invalid_gr";
	strcpy(mg.buf,buff);
    msgsnd(id1,&mg,sizeof(mg)-sizeof(long),0);
}
else
{
for(i=0;i<n;i++)
 {
  if(mg.id != client[i][0] && mg.grpid == client[i][1])
  {
	mg.type=client[i][0];
	msgsnd(id1,&mg,sizeof(mg)-sizeof(long),100);
  }
 }
}
}
	return 0;
}
		
	



