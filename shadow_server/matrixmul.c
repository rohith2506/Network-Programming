//Author: Rama Rao
//Calculating each element by passing a thread for it.
//Passing thread arguments using 'struct arr' (row and a column)
#include<pthread.h>
#include<stdio.h>
int a[5][5],b[5][5],c[5][5];//answer in c
struct arr{
	int row;
	int col;
};
void *function(void* t){ 
struct arr *dat;
dat = (struct arr*)t;
printf("Your sent data is %d %d\n",dat->row,dat->col);
																															
	for(int k=0;k<5;k++){
				c[dat->row][dat->col] = c[dat->row][dat->col]+ a[dat->row][k] * b[k][dat->col];
	}
}

int main(){
	int m,n,m1,n1;
	printf("Enter row and col of 1st and 2nd array\n");
	scanf("%d %d %d %d",&m,&n,&m1,&n1);
	printf("Enter the first array\n");
	for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			scanf("%d",&a[i][j]);
		}
	}
	printf("Enter the second array\n");
	for(int i=0;i<m1;i++){
		for(int j=0;j<n1;j++){
			scanf("%d",&b[i][j]);
		}
	}
printf("This is your first array\n");

for(int i=0;i<m;i++){
		for(int j=0;j<n;j++){
			printf("%d",a[i][j]);
		}
		printf("\n");
	}	

	int i,j,k=0,ro,co;
	pthread_t thread1[m*n1];
	struct arr send[m*n1];
	printf("Yes\n");
	for( ro=0;ro<m;ro++){
		for(co=0;co<n1;co++){
			send[k].row = ro;
			send[k].col = co;
			printf("Sending data %d %d \n",send[k].row,send[k].col);
			pthread_create(&thread1[k],NULL,(void*)function,(void*)&send[k]);
			k++;
		;	}
		}

	for(k=0;k<m*n1;k++)
	pthread_join(thread1[k],NULL);
    printf("%d %d \n",m,n1);
	for(i=0;i<m;i++){
		for(j=0;j<n1;j++){
			printf("%d ",c[i][j]);
		}
		printf("\n");
	}
	return 0;
}



