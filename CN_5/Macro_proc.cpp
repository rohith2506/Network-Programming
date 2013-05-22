/*
Program : Macro processor 
Author  : Rohit

*/

/*
Problem Description:-

1) storing Procedure Name and index of that procedure in Macro Address Table(MAT)
2) Storing Arguments in Argument Address Table(AAT)

*/

/*
GUI:-

1) Display table wise
2) Display Main function
*/


#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <fstream>
#include <map>
using namespace std;

int main(){
   
   ifstream fin;
   fin.open("macro.txt",ios::in);
   cout<<"\t\t##########Output file##########"<<endl;
   cout<<endl;
   cout<<endl;
   string str;
   int count=0;
   int cnt;
   int flag2=0;
   
    map<string,int> mp;
    map<string,int>:: iterator it;

    map<int,string> mp2;
    map<int,string>:: iterator it2;
    
    map<string,string> mt;
    map<string,string> ::iterator it3;
    
   while(getline(fin,str)){
     string foutstring=str;

     char *s2=(char *)str.c_str();

     char *ptr=strtok(s2," (,);+%/={");
     vector<string> v;
     while (ptr != NULL)
     {
       v.push_back(ptr);
       ptr = strtok (NULL, " (,);{");
     }

     if(v[0]=="Macro"){
       mp[v[1]]=count;
       cnt=v.size()-2;
                
       int num_count=1;
     
       for(int i=3;i<v.size();i=i+2){
            string numcount;
            ostringstream con;
            con<<num_count;
            numcount=con.str();
            string s="#"+numcount;
            mt[v[i]]=s;
            num_count++;
       } 
     }

     
     else if(v[0].length()==1 && v[0]!=" "){
            string new_temp;
            for(int i=0;i<v.size();i++){
               if(i%2==0){
               for(it3=mt.begin();it3!=mt.end();it3++){
                  if((*it3).first==v[i])
                     new_temp+=(*it3).second;
               }
            }
              else
                new_temp+=v[i];
            }
        mp2[count]=new_temp;
        count++;
      }
      
     else if(v[0]=="Mend"){} 
     else if(v[0]=="Main"){
          cout<<"\t\t"<<foutstring<<endl;
          }
     else {
        if(v[0]=="return")
           cout<<"\t\t"<<foutstring<<endl;
        else if(v[0]=="End")
           cout<<"\t\t"<<foutstring<<endl;
        else {
        it=mp.find(v[0]);
        
        int count=(*it).second;
        it++;
        int count2=(*it).second;
        
        for(int i=count;i<count2;i++){
                it2=mp2.find(i);
                string temp3=(*it2).second;
                
                string new_temp;
                
                for(int j=0;j<temp3.length();j++){
                        if(temp3[j]=='#'){
                            int x=(int)(temp3[j+1])-48;
                            new_temp+=v[x];
                            j++;
                            }
                        else
                             new_temp+=temp3[j];
                 }
         cout<<"\t\t"<<new_temp<<endl;
         }
         cout<<endl;
   }
}

}
fin.close();
cout<<endl;
cout<<"\t\t#######End of output file#######"<<endl;
cout<<endl;
cout<<endl;
cout<<"\t\t#########Macro Address table##############"<<endl;
cout<<endl;
cout<<endl;
cout<<"\t\tProcedure"<<"         "<<"index"<<endl;
for(it=mp.begin();it!=mp.end();it++)
  cout<<"\t\t"<<(*it).first<<"         "<<(*it).second<<endl;
  

cout<<endl;
cout<<endl;
cout<<"\t\t########Arguments stored table############"<<endl;
cout<<endl;
cout<<endl;
cout<<"\t\tindex"<<"         "<<"arguments"<<endl;
for(it2=mp2.begin();it2!=mp2.end();it2++)
  cout<<"\t\t"<<(*it2).first<<"         "<<(*it2).second<<endl;

return 0;
}



