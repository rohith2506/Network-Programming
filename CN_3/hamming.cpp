/*
here two versions will be  there
* 1)to genarate hamming code 
* 2)to check for error position in hamming code
* @author : rohit
*/

#include <iostream>
#include <stdio.h>
#include <vector>
	
using namespace std;


int pow(int x,int y){
	int prod=1;
	for(int i=0;i<y;i++)
	   prod=prod*x;
	return prod;
}

vector<int> binary(int x,int k){
	 vector<int> v;
	 int cnt=0;
	 while(x>0){
		 int b=x%2;
		 v.push_back(b);
		 x=x/2;
		 cnt++;
	 }
	 for(int j=cnt;j<k;j++)
	    v.push_back(0);	 
	  
	  vector<int> v2;
	 for(int i=v.size()-1;i>=0;i--)
	    v2.push_back(v[i]);
	   
	    
  return v2;
}

int main(){
	cout<<"\n\n\tHAMMING CODE GENARATION"<<endl;

	string gen;
    cin>>gen;
    
    vector<int> g;
    
    for(int i=0;i<gen.length();i++)
		  g.push_back((int)gen[i]-48);    
   
    int m=g.size();
    
    int k=0;
    while((pow(2,k)-k-1)<m)
        k++;
        
   // cout<<k<<endl;
    int len=m+k;  
 
    vector< vector<int> >vec;
    
    for(int i=1;i<=len;i++){
		  vector<int> row;
		  row=binary(i,k);
	      vec.push_back(row);
	 }
	   
    vector< vector<int> >v2;
    
    for(int j=k-1;j>=0;j--){
		vector<int> row;
		for(int i=0;i<len;i++){
		     if(vec[i][j]==1)
		        row.push_back(i);
			}
		v2.push_back(row);
	}
	

  int a[len];
  
  int x=0;
  
  while(pow(2,x)<=len){
	 int p=pow(2,x);
	 a[p-1]=-1;
	 x++;
 }
 
 int j=0;
 for(int i=0;i<len;i++){
	 if(a[i]!=-1){
	    a[i]=g[j];
	    j++;
	}
}

int K=k-1;

for(int i=0;i<len;i++){
	if(a[i]==-1){
		 int cnt=0;
		for(int p=0;p<len;p++)
		   if(vec[p][K]==1)
		      cnt++;
		  if(cnt%2==0)
		     a[i]=0;
		  else
		     a[i]=1;
		     K--;
		 }
  }


cout<<"HAMMING CODE OUTPUT: ";
for(int i=0;i<len;i++)
  cout<<a[i];
  
  cout<<endl;
  
  return 0;
}




   
   
   
   
  
  
   
   
  
   
    
