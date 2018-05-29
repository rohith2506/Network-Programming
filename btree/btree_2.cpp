#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#define size 2

using namespace std;

class btree{
      private:
             struct btnode{
                    bool leaf;
                    int count;
                    int value[(2*size)-1];
                    btnode *ptr[(2*size)];
                    }*root;
      public:
             btree();
             void insert1(int val);
             void split(btnode *&x,int i);
             void insert(btnode *&x,int val);
             ~btree();
             void show(int cnt);
             void display(btnode *&root,int cnt);
             void show1();
             void btree::inorder(btnode *&rt);
             };
             
  btree::btree(){
         btnode *p=new btnode;
         p->count=0;
         p->leaf=true;
        for(int i=1;i<(2*size);i++){
           p->value[i]=0;
           p->ptr[i]=NULL;
           }
         p->ptr[(2*size)]=NULL;
         root=p;
         }
         
  void btree::insert1(int val){
       btnode *r=root;
       if(r->count==(2*size)-1){
            btnode *s=new btnode;
            root=s;
            s->count=0;
            s->leaf=false;
            s->ptr[1]=r;
            split(s,1);
            insert(s,val);
            }
       else
          insert(r,val);
          }
          
 
 void btree::split(btnode *&x,int i){
      btnode *z=new btnode;
      btnode *y=x->ptr[i];
            
      z->leaf=y->leaf;
      z->count=size-1;
      for(int j=1;j<=size-1;j++)
             z->value[j]=y->value[j+size];
       
      if(y->leaf==false){
               for(int j=1;j<=size;j++){
                      z->ptr[j]=y->ptr[j+size];            
                      }
               }
       y->count=size-1;
       
       for(int j=(x->count)+1;j>=i+1;j--)
               x->ptr[j+1]=x->ptr[j];
      
       x->ptr[i+1]=z;
       
       for(int j=(x->count);j>=i;j--)
              x->value[j+1]=x->value[j];
              
       x->value[i]=y->value[size];
              
       x->count=x->count+1;
       }
       
void btree::insert(btnode *&x,int val){
     int i=(x->count);
     if(x->leaf==true){
             while(i>=1 && val<x->value[i]){
                       x->value[i+1]=x->value[i];
                       i--;
                       }
             x->value[i+1]=val;
             x->count=x->count+1;
             }
     else{
          while(i>=1 && val < x->value[i])
                   i=i-1;            
                   i=i+1;                         
          if((x->ptr[i])->count==(2*size)-1){
                //key redistribution here
                 split(x,i);
                 if(val > x->value[i-1])
                     i=i+1;
                     }
              insert(x->ptr[i],val);
              }
     }
     
     
void btree::show(int cnt){
     display(root,cnt);
     }


void btree::display(btnode *&rt,int cnt){
     if(rt!=NULL){
       queue<btnode *> q;
       q.push(rt);
       int CNT=0;
       int lvl=1;
       int prev_cnt=0;
       while(!q.empty()){
                btnode *temp1=q.front();
                q.pop();
                 int i;
                for(i=1;i<=(temp1->count)+1;i++){
                     if(temp1->ptr[i]!=NULL)
                     q.push(temp1->ptr[i]);
                     }
               prev_cnt+=temp1->count+1;
                for(int i=1;i<=temp1->count;i++){
                   cout<<temp1->value[i]<<" ";
                   CNT++;
                   }
                  lvl--;
                  if(lvl==0){
                     cout<<endl;
                     lvl=prev_cnt;
                     prev_cnt=0;
                     }
                  else
                       cout<<"\t";
                   if(CNT==cnt)
                     break;
                }
       }
 }
 
/*
void btree::show1(){
     inorder(root);
     }


void btree::inorder(btnode *&rt){
     if(rt!=NULL){
             int i;
             for(i=1;i<=rt->count;i++){
                     inorder(rt->ptr[i]);
                     cout<<rt->value[i]<<" ";
                     }
             inorder(rt->ptr[i]);
             }
     else return;
}
*/


btree:: ~btree(){
        delete  (root);
        }
                                                       
   int main(){
       btree bt;
       ifstream fin;
       fin.open("btree_in.txt",ios::in);
       int val;
       int cnt=0;
       while(fin>>val){
             bt.insert1(val);
             cnt++;
             }
       cout<<"\t\t######Level order of b-tree######\n\n"<<endl;
       bt.show(cnt);
       cout<<endl;
  //     bt.show1();
       return 0;
       
       }
