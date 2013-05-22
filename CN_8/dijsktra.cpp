#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <algorithm>
#include <stdio.h>
#define MAX 1001
#define MAXINT 10000001


using namespace std;

typedef vector<int> vi,path;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;

vvii G(MAX);
path p(MAX,-1);
vi D(MAX,MAXINT);


void dijsktra(int s){
	set<ii> q;
	D[s]=0;
	q.insert(ii(0,s));
	
	while(!q.empty()){
		ii top = *q.begin();
		q.erase(*q.begin());
		int d= top.first;
		int v= top.second;

	for(vii::const_iterator it=G[v].begin();it!=G[v].end();it++){
		int v2   = it->first;
		int cost = it->second;
	
		if(D[v2] > D[v]+cost){
			if(D[v2]!=MAXINT){
				q.erase(q.find(ii(D[v2],v2)));
				}
			D[v2]=D[v]+cost;
			p[v2]=v;
			q.insert(ii(D[v2],v2));
			}
		}
	}
}

		 
int main(){
	int n,v,src,dest;
	cin>>n>>v>>src>>dest;

	for(int i=0;i<v;i++){
		int x,y,w;
		cin>>x>>y>>w;
		G[x-1].push_back(ii(y-1,w));
		G[y-1].push_back(ii(x-1,w));
		}
	
		
	dijsktra(src-1);
	printf("%d\n",D[dest-1]);
	return 0;
	}
			


