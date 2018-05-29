#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

int main(){
	cout<<"\n\n\tCRC ALGORITHM FOR ERROR CHECK"<<endl;
	
	 cout<<"Enter the input code"<<endl;
	string s;
	cin>>s;
	vector<int> v;
	for(int i=0;i<s.length();i++)
	   v.push_back((int)s[i]-48);
       	   
    cout<<"Enter the generating code"<<endl;
    cin>>s;
    
    vector<int> v2;
    
    for(int i=0;i<s.length();i++)
       v2.push_back((int)s[i]-48);
       
        
    int k=v2.size();
    int n=v.size();
    int a=n+k-1;    
    for(int i=n;i<a;i++)
        v.push_back(0);
          
    vector<int> v3;
   
    for(int i=0;i<n;i++)
       v3.push_back(v[i]);
      
       
   for(int i=0;i<n;i++)
    {
      if(v[i]==0)
      {
       for(int j=i;j<k+i;j++)
         v[j] = v[j]^0;
      }
      else
      {
		  int l=0;
       for(int j=i;j<k+i; j++){
          v[j]=v[j]^v2[l];
          l++;
	  }
      }
   }
     
   cout<<"CRC CODE Generated :";
   
   for(int i=n;i<a;i++)
       cout<<v[i];
       cout<<endl;
       
   cout<<"Total output code :";
   
   for(int i=n;i<a;i++)
      v3[i]=v[i];
   for(int i=0;i<a;i++)
   cout<<v3[i];

  cout<<endl;
  return 0;
} 
   
   
   
   
   
   
   
    
    
    
		   
			  
	
	

