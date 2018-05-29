#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <stdlib.h>


struct msg{
		long type;
		int buff;	
		int id;
		int mesg;
	};

struct tnode {
   char val;
   struct tnode * right, * left;
};

typedef struct tnode node;

void insert(node ** tree, node * item) {
   if(!(*tree)) {
      *tree = item;
      return;
   }
   if(item->val<(*tree)->val)
      insert(&(*tree)->left, item);
   else if(item->val>(*tree)->val)
      insert(&(*tree)->right, item);
   else{}
}
            
int search(char key,node * tree) {
	if(tree!=0){
    if(tree->val==key)
       return 1;
    else if(tree->val>key) 
       {search(key,tree->left);}
    else if(tree->val<key) 
       {search(key,tree->right);}
  else{}
   }
   else
     return 0;
}



int main(int argc,char *argv[]) {
   struct msg mg;
   node * curr, * root;
   int i;

   root = NULL;

  int key=ftok("errj",3245);
  int id1=msgget(key,IPC_CREAT | 0666);
  
   for(i=1;i<=10;i++) {
      curr = (node *)malloc(sizeof(node));
      curr->left = NULL;
      curr->right = NULL;
      curr->val=i;
      insert(&root, curr);
   }  
          while(1){
		   
           msgrcv(id1,&mg,sizeof(mg)-sizeof(long),10,0);
           printf("Message received\n");
           int b=mg.buff;
           int p=search(b,root);
           if(p==1){
             //mg.mesg=2;
             mg.type=100;
             mg.id=10;
             msgsnd(id1,&mg,sizeof(mg)-sizeof(long),0);
		     }   
	   }
                    
   return 0;
}

			    
   







  
