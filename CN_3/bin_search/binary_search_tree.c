#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<pthread.h>

int choice =-1;
pid_t pid;
pthread_mutex_t tree;

struct node
{
	int key;
	struct node* left;
	struct node* right;
}*root=NULL;

struct node* insert(struct node* bn,int t)
{
	if(bn==NULL)
	{
		bn =(struct node*)malloc(sizeof(struct node));
		bn->key=t;
		bn->left=NULL;
		bn->right=NULL;
		return bn;
	}
	if(t<bn->key)
	{
		bn->left = insert(bn->left,t);
	}
	else if(t>bn->key)
	{
		bn->right = insert(bn->right,t);
	}
	
	return bn;
}

void searchtree(struct node* bn,int n)
{	
	if(bn!=NULL)
	{
		if(n<bn->key)
		{
		    searchtree(bn->left,n);
		}
		else if(n>bn->key)
		{
		    searchtree(bn->right,n);
		}
		else
		    printf("element found\n");
		return;
	}
    	else
        	printf("element not present\n");
}

struct node* findmin(struct node* bn)
{
    if(bn->left==NULL)
        return bn;
    findmin(bn->left);
}

void remov(struct node* bn,int n)
{
    if(bn==NULL)
    {
        printf("element not there in tree\n");
        return;
    }
    if(n<bn->key)
        remov(bn->left,n);
    else if(n>bn->key)
        remov(bn->right,n);
    else if(bn->left!=NULL&&bn->right!=NULL)
    {

        bn->key=findmin(bn->right)->key;
        remov(bn->right,bn->key);
    }
    else
    {
        struct node *old=bn;
        bn=(bn->left!=NULL)?bn->left:bn->right;
        free(old);
    }
}

void* maketree()
{
	pthread_mutex_lock(&tree);
	printf("press -1 to terminate process of maketree\n");
	int flag;
	printf("enter number\n");
	scanf("%d",&flag);	
	do
	{
		root = insert(root,flag);
		printf("enter number\n");
		scanf("%d",&flag);	
	}while(flag!=-1);
	pthread_mutex_unlock(&tree);
}

void* search()
{
	pthread_mutex_lock(&tree);
	int t;
	printf("enter no. to be searched\n");
		scanf("%d",&t);
	searchtree(root,t);
	pthread_mutex_unlock(&tree);
} 

void* deltree()
{
	pthread_mutex_lock(&tree);
	int t;
	printf("enter no. to be deleted\n");
		scanf("%d",&t);
	remov(root,t);
	kill(pid,SIGINT);
	pthread_mutex_unlock(&tree);
} 

void printtree(struct node* bn)
{
	if(bn!=NULL)
	{
		printtree(bn->left);
		printf("%d ",bn->key);
		printtree(bn->right);
	}
	
}

void print()
{
	printtree(root);
}

int main()
{
	pid=getpid();
	signal(SIGINT,print);
	root=NULL;
	int flag=-2;
	pthread_t tid[4];
	pthread_create(&tid[0],NULL,&maketree,NULL);
	pthread_create(&tid[1],NULL,&maketree,NULL);
	pthread_create(&tid[2],NULL,&search,NULL);
	pthread_create(&tid[3],NULL,&deltree,NULL);
	
	while(1)
	{
		printf("1:insert\n2:search\n3:delete\n\"\\ ENTER CHOICE \\\"\n");
		scanf("%d",&choice);
	}
	return 0;	
}
