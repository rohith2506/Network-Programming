#include <pthread.h>
#include <stdio.h>
#define NUM 5

struct process{
	int burst_time;
	int process_id;
	//int wait_time;
};
struct process p[NUM];
pthread_cond_t con[NUM];
pthread_mutex_t mutex;
int quantum,count=0;

void *rr(void* t){
int pos = *((int*) t);

	while(1){
		
		
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&con[pos],&mutex);
		if(count==NUM)
			exit(0);
		
		
	

	else if(p[pos].burst_time >= quantum)
		{
			sleep(quantum);
			p[pos].burst_time -= quantum;
		}
	else{
		sleep(p[pos].burst_time);
		p[pos].burst_time  = 0;
		count++;
	} 
	
	printf("Process %d is executing and its burst time is %d \n",pos,p[pos].burst_time);
	
	pthread_cond_signal(&con[(pos+1)%NUM]);
	pthread_mutex_unlock(&mutex);
	sleep(2);

}

}

int main(){

	pthread_t *pthread = (pthread_t *)malloc(sizeof(pthread_t)*10);
	
	pthread_mutex_init(&mutex,NULL);
		printf("Enter process burst times(5 processes)\n");
	for(int i=0;i<NUM;i++){
		scanf("%d",&p[i].burst_time);
		}
	printf("Enter your quantum\n");	
	scanf("%d",&quantum); 
	for(int i=0;i<NUM;i++)
		pthread_cond_init(&con[i],NULL);

int tmp[NUM];
for(int i=0;i<NUM;i++){
	tmp[i]=i;
	pthread_create(&pthread[i],NULL,(void*)rr,(void*)&tmp[i]);
}
sleep(2);
pthread_mutex_lock(&mutex);
pthread_cond_signal(&con[0]);
pthread_mutex_unlock(&mutex);
for(int i=0;i<NUM;i++)
	pthread_join(pthread[i],NULL);
return 0;

}
