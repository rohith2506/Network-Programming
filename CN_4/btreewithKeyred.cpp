//This is B2 tree
#include<iostream>
using namespace std;
#define LEN 5
#define LEAST -99999
struct node{
	int dat[LEN];
	struct node* a[LEN+1];
	int count;
	int greatest;
	int small;
};
struct overpair{
	int median;
	struct node* overnode1;
	struct node* overnode2;
};
int flag=0;
node* root=NULL;

int findmedian(struct node* t){
		if(t->count%2 == 0)
		return t->dat[(t->count/2)-1];
		else
		return t->dat[(t->count+1)/2 - 1];
}
void sort(struct node* &t,int l,int item)
{
	int i,j;
	for(i=0;i<t->count;i++)
		for(j=i+1;j<t->count;j++)
			if(t->dat[i] > t->dat[j])       /*************sorting data items ***********/
			{
			int data=t->dat[i];
			t->dat[i]=t->dat[j];
			t->dat[j]=data;
			}
	if(l==0)
	{
		node* temp=t->a[i];
	
		while(t->dat[i-1]!=item)				/************sorting links******************/
		{
		t->a[i]=t->a[i-1];
		i--;
		}
		t->a[i]=temp;
	}	
}
void leftshift(node* temp){
int i;
for(i=0;i<temp->count-1;i++)
	temp->dat[i]=temp->dat[i+1];
for(i=0;i<temp->count;i++)
	temp->a[i]=temp->a[i+1];
	temp->a[i]=NULL;
	}
void display2(node* temp,int level)
{
if(temp==NULL)
return;
if(level==1)
{
	cout<<"[";
	for(int i=0;i<temp->count;i++)
{
cout<<temp->dat[i]<<",";
}
cout<<"\b] ";
}
else
for(int i=0;i<temp->count+1;i++)
display2(temp->a[i],level-1);
}


int maxht(struct node* temp){
	if(temp==NULL)
	return 0;
	else
	return  maxht(temp->a[0])+1;
}
void display(node* t){
	int ht =maxht(root);
	cout<<endl;
	for(int i=1;i<=ht;i++)
	{
		display2(root,i);
		cout<<endl;
	}
	cout<<endl;
}
				
node* split(node* &t,int med)
{

	node* p =new node;
	p->count=0;
	int i=0,k=0,j=0;

	while(t->dat[i]!=med)i++;
	t->count=i;
	j=i+1;
	i++;
	while(i<LEN)
	{	
		p->count++;
		p->dat[k++]=t->dat[i];
		i++;
	}	
	k=0;
	while(j<=LEN)
	{	
	
		p->a[k++]=t->a[j];
	j++;
	}
	return p;
}

overpair*  insert(node* &t,int item)
{

   if(t==NULL)
   {
   
		   t=new node;
		   t->dat[0]=item;
			t->count=1;
			t->a[0]=NULL;
			overpair* o=new overpair;
			o->median = -1;
			o->overnode1=NULL;
			o->overnode2=NULL;
     	return o;
   }
	else
	{
		int n;
		int i=0,flag=0,dontsplit=0,l,r;
	while(i<t->count)
		{	
			flag=0;dontsplit=0;l=0;r=0;
			//cout<<t->count<<endl;
			if( (item < t->dat[i] && t->a[i]!=NULL) )
			{
				flag=1;
				n=i;
			}
			if( item > t->dat[i] && t->a[i+1]!=NULL && i==t->count-1)
			{
				flag=1;
				n=i+1;
			}
			if(flag==1)
			{
					overpair* o =  insert(t->a[n],item);
					if(o->overnode1!=NULL)
					{
						if(n!=0 && n!=t->count)
						{l==1;r==1;}
						else if(n==0)
									{r==1;l=0;}
									else if(n==t->count)
												{l=1;r=0;}
						if(l==1){					
						if( t->a[n-1]->count!=LEN-1)
						{
						int temp=t->dat[n-1];
						t->dat[n-1]=o->overnode1->small;
						node* alias=t->a[n-1];
						alias->dat[alias->count]=temp;
						alias->a[alias->count+1]=o->overnode1->a[0];
						o->overnode1->a[0]=NULL;
						alias->count++;
						leftshift(o->overnode1);
						o->overnode1->count--;
						sort(alias,0,temp);
						dontsplit=1;
						
						}
						}
						else if(r==1){
						if(t->a[n+1]->count != LEN-1)
						{
						int temp = t->dat[n+1];
						t->dat[n+1]=o->overnode1->greatest;
						node* alias=t->a[n+1];
						alias->dat[alias->count]=temp;
						alias->a[alias->count+1]=o->overnode1->a[o->overnode1->count];
						alias->count++;
						sort(alias,0,temp);
						o->overnode1->a[o->overnode1->count]=NULL;
						o->overnode1->count--;
						dontsplit=1;
					
						}
						}
					if(dontsplit==1){
					overpair* o=new overpair;
					o->median = -1;
					//	o->overnode1->small=-1;
					//o->overnode1->greatest = -1;
					o->overnode1=NULL;
					o->overnode2=NULL;
					return o;
					}
						else{				
						o->overnode2=split(o->overnode1,o->median);				
						t->dat[t->count]=o->median;
						t->a[t->count+1]=o->overnode2;
						t->count++;
						sort(t,0,o->median);	
					if(t->count==LEN)
					{
					overpair* o=new overpair;
					int med = findmedian(t);
					//node* k =split(t,med);
					//cout<<"Splitting completed"<<endl;
					o->median = med;
					
					
					o->overnode1=t;
					o->overnode1->small=t->dat[0];
					o->overnode1->greatest = t->dat[t->count-1];
					o->overnode2=NULL;
					return o;
					}
					else{
						//display(root);
						overpair* o=new overpair;
						//o->overnode1->small=-1;
					//o->overnode1->greatest = -1;
					o->median=-1;
					o->overnode1=NULL;
					o->overnode2=NULL;
					return o;
					}	
					}	
				}
				else{
					overpair* o=new overpair;
					//	o->overnode1->small=-1;
					//o->overnode1->greatest = -1;
					o->median=-1;
					o->overnode1=NULL;
					o->overnode2=NULL;
					return o;
					}
			}		
			else
			i++;
		}
		t->dat[i]=item;
		t->a[i]=NULL;
		if(i==LEN-1)
		t->a[LEN]=NULL;
		t->count++;
		sort(t,1,item);
		if(t->count==LEN)
		{
		
		overpair* o=new overpair;
		int med = findmedian(t);
		//cout<<"Found median "<<med<<endl;
		
		o->median = med;
		o->overnode1=t;
		o->overnode1->small=t->dat[0];
		o->overnode1->greatest=t->dat[t->count-1];
		o->overnode2=NULL;
		//display(root);
		return o;
		}
		else
		{
		//display(root);
		overpair* o=new overpair;
		o->median = -1;
		//o->overnode1->greatest=-1;
		//o->overnode1->small=-1;
		o->overnode1=NULL;
		o->overnode2=NULL;
		return o;
		}
	
	}

}



int main()
{
int n,item;
cout<<"Enter Data and -1 In The End"<<endl;
cin>>item;
while(item!=-1)
{
 overpair* o= insert(root,item);
if(o->median!=-1)
	{	
		node* t =new node;
		t->count=0;
		t->a[0]=o->overnode1;
		o->overnode2=split(o->overnode1,o->median);
		t->a[1]=o->overnode2;
		t->dat[0]=o->median;
		root=t;
		t->count++;
		cout<<"New root created "<<root->dat[0]<<endl;
	}
	display(root);
	cin>>item;
}
cout<<"You have created the tree"<<endl;
display(root);
return 0;
}
