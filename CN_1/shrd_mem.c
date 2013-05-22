#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/wait.h>

int main()
{
struct sembuf lck,unlck;

int semid1=semget(ftok("in23",123),1,IPC_CREAT | 0666);
int semid2=semget(ftok("in223",127),1,IPC_CREAT | 0666);

semctl(semid1,0,SETVAL,1);
semctl(semid2,0,SETVAL,1);

lck.sem_num=0;
lck.sem_op=-1
lck.sem_flg=0;

unlck.sem_num=0;
unlck.sem_op=1;
unlck.sem_flg=0;

int shmid1=shmget(ftok(("in123"),123),10,IPC_CREAT | 0666);
int shmid2=shmget(ftok(("in456"),15),10,IPC_CREAT | 0666);

int* n1=(int*)shmat(shmid1,(void*)0,0);
int* n2=(int*)shmat(shmid2,(void*)0,0);

n1[0]=0;
n2[0]=1;
int c=fork();

if(c==0)
{
while(n1[0]<10 && n2[0]<10)
{
      semop(semid1,&lck,1);
      n1[0]=n2[0]+1;
      printf("n1 is increased by n2+1 in child process val is %d\n",n1[0]);
      semop(semid1,&unlck,1);
      semop(semid2,&lck,1);
      n2[0]=n1[0]+1;
      printf("n2 is increased by n1+1 in child process  val is %d\n",n2[0]);
      semop(semid2,&unlck,1);
      sleep(2);
}

}

if(c>0)
{
  while(n1[0]<10 && n2[0]<10)
  {
         semop(semid1,&lck,1);
         n1[0]=n2[0]+1;
         printf("n1 is increased by n2+1 in parent process val is %d\n",n1[0]);
         semop(semid1,&unlck,1);
         semop(semid2,&lck,1);
         n2[0]=n1[0]+1;
         printf("n2 is increased by n1+1 in parent process val is %d\n",n2[0]);
         semop(semid2,&unlck,1);
         sleep(1);
   }
wait(NULL);
}

return 0;
}



