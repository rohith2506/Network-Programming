#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <signal.h>
#include <stdlib.h>

pid_t pid;

using namespace std;

pthread_mutex_t mutex;

class btree{
	struct bnode{
		int data;
		bnode *left;
		bnode *right;
	}*root;
	public:
	   btree();
	   void insert1(int data);
	   void insert(bnode *&t,int num);
	   void remove1(int data);
	   void remove(bnode *&t,int num);
	   void search1(int num);
	   void search(bnode *&t,int num);
	   bnode *findmax(bnode *t);
	   void print(bnode *&t,int n);
	   int height(bnode *&t);
	   void print1(int n);
   };


  btree::btree(){
	  root=NULL;
  }
  
  void btree::search1(int num){
	  search(root,num);
  }
  
 void btree::search(bnode *&t,int num){
	 if(t!=NULL){
	 if(t->data<num)
	    search(t->right,num);
	 else if(t->data>num)
	    search(t->left,num);
	 else
		 cout<<"Required number found "<<endl;
		return ;
	 }
	 else
	   cout<<"Required number not found"<<endl;
  }
  
  void btree::insert1(int num){
	  insert(root,num);
  }
  
  void btree::insert(bnode *&t,int num){
	  if(t==NULL){
		  t=new bnode;
		  t->data=num;
		  t->left=NULL;
		  t->right=NULL;
	  }
	  else if(t->data>=num)
	     insert(t->left,num);
	  else if(t->data<num)
	     insert(t->right,num);
	  else{}
  }
  
  
void *insert_func(void *k){
	btree bt;
	pthread_mutex_lock(&mutex);
	cout<<"Enter -1 to terminate"<<endl;
	int n;
	do{
	cout<<"insert data"<<endl;
	cin>>n;
	bt.insert1(n);
      }while(n!=-1);
	pthread_mutex_unlock(&mutex);
	sleep(1);
  }
 
void *search_func(void *k){
	  btree bt;
	  pthread_mutex_lock(&mutex);
	  int n2;
	      cout<<"search for require element"<<endl;
	  cin>>n2;
	  bt.search1(n2);
	  pthread_mutex_unlock(&mutex);
  }
  
void *remove_func(void *k){
	btree bt;
	pthread_mutex_lock(&mutex);
	  int n2;
	      cout<<"delete the required element"<<endl;
	  cin>>n2;
	  bt.remove1(n2);
	  kill(pid,SIGINT);
	  pthread_mutex_unlock(&mutex);
  }
  
void btree::remove1(int num){
	remove(root,num);
}

void btree::remove(bnode *&t,int num){
	  if(t==NULL){
	    cout<<"Element not present"<<endl;
        return ;
    	}
     else if(t->data>num)
        remove(t->left,num);
     else if(t->data<num)
        remove(t->right,num);
     else{
		 if(t->left!=NULL&&t->right!=NULL){
			 t->data=findmax(t->left)->data;
			 remove(t->left,num);
		 }
		else{
		bnode *temp=t;
		   if(t->left==NULL)
		    t=t->right;
		   if(t->right==NULL)
		    t=t->left;
		 delete temp;  
	   }
  }
}

bnode *btree::findmax(bnode *t){
	   if(t!=NULL){
		while(t!=NULL)
	      t=t->left;
	  }
	  else
        return t;
	}
	
void btree::print1(int n){
	print(root,0);
}
	
void bulidtree_func(int){
	btree bt;
	bt.print1(0);
}

void btree::print(bnode *&t,int n){
	if(t!=NULL){
		int i=height(t->left);
		int j;
		for(j=0;j<2*i+2+n;j++)
		   cout<<" ";
		   cout<<t->data<<endl;
		for(j=0;j<2*i+n+2;j++){
		  if(j==2*i+n+1)
		     printf("/ \\");
		    cout<<" ";		
		}
		cout<<endl;
		print(t->left,n);
		print(t->right,2*i+n+3);
		return;
	}
return;
}

int btree::height(bnode *&t){
    int h,lft,rgt;
     if(t!=NULL){
         lft=height(t->left);
         rgt=height(t->right);
        if(lft>=rgt)
          h=lft;
        else
          h=rgt;
        return h+1;
     }
else if(t==NULL)
return -1;
}

		 
int main(){
	pid=getpid();
			signal(buildtree_func,SIGINT);
	pthread_t pt[4];
	//calling two threads for inserting data which are acting on only one function
	
	while(1){
	  pthread_create(&pt[0],NULL,insert_func,NULL);
	  pthread_create(&pt[1],NULL,insert_func,NULL);
      pthread_create(&pt[2],NULL,search_func,NULL);	   
	  pthread_create(&pt[3],NULL,remove_func,NULL);
      }
      
    pthread_join(pt[0],NULL);
    pthread_join(pt[1],NULL);
    pthread_join(pt[2],NULL);
    pthread_join(pt[3],NULL);
    
    return 0;
}
  
	  
	  
	 
	
	
