#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/msg.h>
#include<poll.h>
#include<signal.h>
struct msg
{
long type;
char buf[100];
};
struct msg mesg;
pthread_mutex_t m,m1;
pthread_cond_t cond1,cond2,cond3,cond4,cond[6];


void server(){
int fd[6],i,k=0;
char buf[100];
struct pollfd Client[6];
printf("This is chat server\n");
mkfifo("client1",0777);
fd[0]=open("client1",O_WRONLY | O_NONBLOCK);

mkfifo("client2",0777);
fd[1]=open("client2",O_RDONLY | O_NONBLOCK);

mkfifo("client3",0777);
fd[2]=open("client3",O_WRONLY | O_NONBLOCK);
	
mkfifo("client4",0777);
fd[3]=open("client4",O_RDONLY | O_NONBLOCK);

mkfifo("client5",0777);
fd[4]=open("client5",O_WRONLY | O_NONBLOCK);
	
mkfifo("client6",0777);
fd[5]=open("client6",O_RDONLY | O_NONBLOCK);
	
for(i=0;i<6;i++)
Client[i].fd=fd[i];

for(i=0;i<6;i+=2)
Client[i].events=POLLOUT;

for(i=1;i<6;i+=2)
Client[i].events=POLLIN;

int n;
while(1)
{
if(poll(Client,6,-1)>0)
{	
for(i=1;i<6;i+=2)
if(Client[i].revents & POLLIN)
{
n =read(Client[i].fd,buf,100);
printf("Server received the message\n");
write(1,buf,n);
pthread_cond_signal(&cond[(i+1)%6]);
pthread_cond_signal(&cond[(i+3)%6])
}
for(i=0;i<6;i+=2)
 if(Client[i].revents & POLLOUT)
{
write(fd[i],buf,n);
    } 
   }
  }
}

void clientsend(void* ptr)
{	
while(1){

int fd[4];
char buf[100];
if( *(int*)ptr ==0)
{
printf("Clientsend I is %d \n",*(int*)ptr);
mkfifo("text2",0777);
fd[*(int*)ptr]=open("text2",O_WRONLY);
pthread_mutex_lock(&m);
pthread_cond_wait(&cond1,&m);
printf("Here\n");
}
if( *(int*)ptr ==1)
{
printf("clientsend I is %d \n",*(int*)ptr);

mkfifo("text4",0777);
fd[*(int*)ptr]=open("text4",O_WRONLY);
pthread_mutex_lock(&m);
pthread_cond_wait(&cond2,&m);
printf("Here2\n");
}
if( *(int*)ptr ==2)
{
printf("clientsend I is %d \n",*(int*)ptr);
// pthread_mutex_lock(&m);
mkfifo("text6",0777);
fd[*(int*)ptr]=open("text6",O_WRONLY);
pthread_mutex_lock(&m);
pthread_cond_wait(&cond3,&m);
}
printf("Then enter some message\n");
int n=read(0,buf,100);
write(fd[*(int*)ptr],buf,n);
pthread_cond_signal(&cond4);
pthread_mutex_unlock(&m);
}
}

void clientrec(void* ptr)
{
while(1){
// // pthread_mutex_lock(&m1);
int *i=(int*)ptr;
int fd[4];
char buf[100];
if( *(int*)ptr ==0)
{

printf("Client I is %d \n",*(int*)ptr);
mkfifo("text1",0777);
fd[*(int*)ptr]=open("text1",O_RDONLY | O_NONBLOCK);
pthread_mutex_lock(&m);
pthread_cond_wait(&cond[0],&m1);
}
if( *(int*)ptr ==1)
{
printf("Client I is %d \n",*i);
// pthread_mutex_lock(&m1);
mkfifo("text3",0777);
fd[*(int*)ptr]=open("text3",O_RDONLY | O_NONBLOCK);
pthread_mutex_lock(&m);
pthread_cond_wait(&cond[2],&m1);
}
if( *(int*)ptr ==2)
{
printf("Client I is %d \n",*i);
// pthread_mutex_lock(&m1);
mkfifo("text5",0777);
fd[*(int*)ptr]=open("text5",O_RDONLY | O_NONBLOCK);
pthread_mutex_lock(&m);
pthread_cond_wait(&cond[4],&m1);
}
int n =read(fd[*(int*)ptr],buf,100);
printf("Message received from server is\n");
write(1,buf,n);
pthread_cond_signal(&cond4);
pthread_mutex_unlock(&m1);
}	
}


void mesgservsend()
{	
int i;
int msgid=msgget(ftok("SerCliThread.c",3),IPC_CREAT | 0777);
//pthread_cond_wait();
for(i=1;i<=2;i++)
{
mesg.type=i;
msgsnd(msgid,&mesg,sizeof(struct msg)-sizeof(long),0);
}
}

void mesgservrec()
{
int i;
int msgid=msgget(ftok("SerCliThread.c",3),IPC_CREAT | 0777);
pthread_cond_wait(&cond3,&m);
msgrcv(msgid,&mesg,sizeof(struct msg)-sizeof(long),3,0);
printf("Message received is %s\n",mesg.buf);
}


void mesgclientrec(void* ptr)
{	int *i = (int*)ptr;
int msgid=msgget(ftok("SerCliThread.c",3),IPC_CREAT | 0777);
if(*i==1)
pthread_cond_wait(&cond1);
if(*i==2)
pthread_cond_wait(&cond2);
msgrcv(msgid,&mesg,sizeof(struct msg)-sizeof(long),*i,0);
}

void mesgclientsend(void* ptr)
{
int *i = (int*)ptr;
int msgid=msgget(ftok("SerCliThread.c",3),IPC_CREAT | 0777);
if(*i==1)
pthread_cond_wait(&cond1,&m);
if(*i==2)
pthread_cond_wait(&cond2,&m);
int  n =read(0,mesg.buf,100);
mesg.type=3;
pthread_cond_signal(&cond3);
msgsnd(msgid,&mesg,sizeof(struct msg)-sizeof(long),0);
}


int main(){
 
pthread_t t1,t2[4],t3,t4,t5[2],t6[2],t7[4];
int n,i,k;
char ch[1];
while(1){
printf("Enter your choice.1:Private Chat  2:Public Chat and count of clients.\n");
scanf("%d",&n);
if(n==1)
{
pthread_create(&t1,NULL,(void*)&server,NULL);
for(i=0;i<3;i++)
pthread_create(&t2[i],NULL,(void*)&clientsend,(void*)&i);
for(i=0;i<3;i++)
pthread_create(&t7[i],NULL,(void*)&clientrec,(void*)&i);
printf("Client 1 or 2 or 3\n");
read(0,ch,1);
k=(int)ch -48;
if(k==1)
pthread_cond_signal(&cond1);
else if(k==2)
pthread_cond_signal(&cond2);
else if(k==3)
pthread_cond_signal(&cond3);
pthread_cond_wait(&cond4,&m);
}
else
{
  pthread_create(&t3,NULL,(void*)&mesgservsend,NULL);
  pthread_create(&t4,NULL,(void*)&mesgservrec,NULL);
for(i=0;i<1;i++)
{
   pthread_create(&t5[i],NULL,(void*)&mesgclientsend,(void*)&i);
   pthread_create(&t6[i],NULL,(void*)&mesgclientrec,(void*)&i);
}
printf("Client 1 or 2\n");
scanf("%d",&k);

if(k==1)
pthread_cond_signal(&cond1);
else if (k==2)
pthread_cond_signal(&cond2);
}
}
return 0;
}
