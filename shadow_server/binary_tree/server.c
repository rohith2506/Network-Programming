#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
struct msg
{
	long type;
	int buff;
	int id;
};
int client[3];
int n;
int main(int argc,char *argv[])
{
	printf("Main Server\n");
	int key1=ftok("ppipe23",123);
	int id2=msgget(key1,IPC_CREAT | 0666);
	struct msg mg;
	client[0]=10;
	client[1]=20;
	client[2]=30;
	  
		//printf("binary tree constructed\n");
		
		while(1)
		{   				
		  printf("Send your message you want to check\n");	
		  scanf("%d\n",&mg.buff);
		int i;
		for(i=0;i<n;i++){
		      mg.type = client[i];
		      msgsnd(id2,&mg,sizeof(mg)-sizeof(long),0);
		  }
		for(i=0;i<n;i++){      
		      msgrcv(id2,&mg,sizeof(mg)-sizeof(long),100,0);
		if(mg.id==10)
		   printf("your massage is present in Process 1\n");
		else if(mg.id==20)
		   printf("Your message is present in Process 2\n");
		else
		   printf("Your message is present in Process 3\n");
		}
		}	           
	return 0;
 }
		
	

