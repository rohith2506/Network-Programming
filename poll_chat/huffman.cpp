#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <queue>
#include <vector>
using namespace std;

    class node
    {
      public:
        int freq;
        char ch[10];
        node* right;
        node* left;
    };
  
   struct cmp
   {
    bool operator()(const node* lhs, const node* rhs) const { return lhs->freq > rhs->freq; }
   };
 
 
    node* insert(char* a, int x)
    {
        node* temp= new node;
        temp->freq = x;
        strcpy( temp->ch , a);
        temp->right = NULL;
        temp->left = NULL;
        return temp;
    }
 
    void gencode(node* temp, int c[], int n)
    {
           if ((temp->left == NULL) && (temp->right == NULL))
           {
            cout<<" letter:   "<< temp->ch<<"    ";
               for (int i = 0; i < n; i++)
              {
                cout<< c[i];
              }
               cout<<endl;
        }
        else
        {
            c[n] = 1;
            n++;
            gencode(temp->left, c, n);
            c[n - 1] = 0;
            gencode(temp->right, c, n);
        }
 
    }
 
        void del(node * root)
    {
        if(root!=NULL)
        {
            del(root->left);
            del(root->right);
            delete root;
        }
 
    } 
 
     
 
int main()
 
{
        cout<<"\t#########HUFFMAN CODE###########"<<endl;
        node* nptr;
        node *temp1;
        node * temp2;
        node* a[20];
 
        int n,freq,b,c[15];
 
        char str[10];
        cout<<"\n\n\tEnter the number of letters in your code :";
        cin>>n;
        cout<<endl;
        priority_queue<node*, vector<node*>, cmp> pq;
        for (int i = 0; i < n; i++)
        {
            cout<<"\n\tEnter the string: " ;
            cin>>str;
            cout<<"\n\tEnter frequency: ";
            cin>>freq;
            a[i] =insert(str, freq);
            pq.push(a[i]);
        }
        
       while (n > 1)
        {
            temp1=pq.top();
            pq.pop();
            temp2=pq.top();
            pq.pop();
            b=(temp1->freq)+(temp2->freq);
            strcpy(str,temp1->ch);
            strcat(str,temp2->ch);
            nptr = insert(str, b);
            nptr->right = temp2;
            nptr->left = temp1;
            pq.push(nptr);
            n--;
         }
         
        gencode(pq.top(), c, 0);
        return 0;
}
