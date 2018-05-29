#include <iostream>
#include <stdio.h>
#include <set>
#include <vector>
#include <fstream>
using namespace std;
 
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
 
const int MAX = 1001;
const int MAXINT = 1000000000;
 
int n;
vvii G(MAX);
vi D(MAX, MAXINT);
 
void Dijkstra(int s)
{
    set<ii> Q;
    D[s] = 0;
    Q.insert(ii(0,s));
 
    while(!Q.empty())
    {
        ii top = *Q.begin();
        Q.erase(Q.begin());
        int v = top.second;
        int d = top.first;
 
        for (vii::iterator it = G[v].begin(); it != G[v].end(); it++)
        {
            cout<<"iam here"<<endl;
            int v2 = it->first;
            int cost = it->second;
            if (D[v2] > D[v] + cost)
            {
                if (D[v2] != 1000000000)
                {
                    Q.erase(Q.find(ii(D[v2], v2)));
                }
                D[v2] = D[v] + cost;
                Q.insert(ii(D[v2], v2));
            }
        }
    }
}
 
int main()
{
    ofstream fout;
    fout.open("out.txt",ios::out);
    int m, s, t = 0;
    scanf("%d %d %d %d", &n, &m, &s, &t);
 
    for (int i = 0; i < m; i++)
    {
        int a, b, w = 0;
        scanf("%d %d %d", &a, &b, &w);
        G[a - 1].push_back(ii(b - 1, w));
        G[b - 1].push_back(ii(a - 1, w));
    }

    for(int i=0;i<G.size();i++){
        for(int j=0;j<G[i].size();j++){
            ii temp = G[i][i];
            fout<<"("<<temp.first<<","<<temp.second<<")"<<" ";    
        }
        cout<<endl;
    }


    Dijkstra(s - 1);
 
    printf("%d\n", D[t - 1]);
 
    return 0;
}