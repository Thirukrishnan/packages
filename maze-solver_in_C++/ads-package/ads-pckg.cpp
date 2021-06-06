#include<bits/stdc++.h>
using namespace std;

#define inf 1000
int countp=1;
int N=0;

struct Node
{
    int id;
    int key1,key2;
    Node *left;
    Node *right;
    int height;
}*root=NULL;


int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b)
{
    return (a > b)? a : b;
}

bool greaterthan(int key1,int key2,int key3,int key4)
{
    if(key1>key3)
    {
        return true;
    }
    else if(key1==key3)
    {
        if(key2>key4)
            return true;
        else
            return false;
    }
    return false;
}

bool lessthan(int key1,int key2,int key3,int key4)
{
    if(key1<key3)
    {
        return true;
    }
    else if(key1==key3)
    {
        if(key2<key4)
            return true;
        else
            return false;
    }
    return false;
}

Node* newNode(int a,int b,int k)
{
    Node* node = new Node();
    node->key1 = a;
    node->key2=b;
    node->id=k;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left),
                    height(y->right)) + 1;
    x->height = max(height(x->left),
                    height(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left),
                    height(x->right)) + 1;
    y->height = max(height(y->left),
                    height(y->right)) + 1;

    return y;
}

Node *search(Node *root,int key)
{
    while(root!=NULL)
    {
        if(key<root->id)
            root=root->left;
        else if(key>root->id)
            root=root->right;
        else
            return root;
    }
}

int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}


Node* insert(Node* node, int a,int b,int id)
{
    if (node == NULL)
        return(newNode(a,b,id));

    if (a< node->key1)
        node->left = insert(node->left, a,b,id);
    else if (a > node->key1)
        node->right = insert(node->right, a,b,id);
    else if(a==node->key1)
    {
        if(b<node->key2)
            node->left=insert(node->left,a,b,id);
        else if(b> node->key2)
        {
            node->right=insert(node->right,a,b,id);
        }
        else
            return node;
    }

    node->height = 1 + max(height(node->left),
                        height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && (a < node->left->key1 || b<node->left->key2))
        return rightRotate(node);

    if (balance < -1 && (a > node->right->key1 || b>node->right->key2))
        return leftRotate(node);

    if (balance > 1 && (a > node->left->key1 || b > node->left->key2))
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && (a < node->right->key1 || b < node->right->key2 ))
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

int key(Node* root,int a,int b)
{
    Node *root1=root;
    while(root1!=NULL)
    {
        if(lessthan(a,b,root1->key1,root1->key2))
            root1=root1->left;
        else if(greaterthan(a,b,root1->key1,root1->key2))
            root1=root1->right;
        else
            return root1->id;
    }
    return 0;
}

void Inorder(Node *root)
{
    if(root == NULL)
        return;
    Inorder(root->left);
    cout<<root->id<<":"<< root->key1<<" "<<root->key2<<endl;
    Inorder(root->right);
}


int minDistance(int dist[],
                bool sptSet[])
{

    int min = inf, min_index;

    for (int v = 0; v < countp; v++)
        if (sptSet[v] == false &&
                   dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

string* printPath(int parent[], int j, string path[20][20])
{

    if (parent[j] == - 1)
        return NULL;

    Node *node=search(root,j+1);
    path[node->key1][node->key2]="0";
    printPath(parent, parent[j],path);
    cout<<" "<<j;
    return *path;

}


int printSolution(int dist[],int n,int parent[])
{
    string path[20][20];
    string *path1;

    int src = 0,i,j;
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
            path[i][j]="1";
    }

    cout<<"\t\t\t\t\t    SOLUTION PAGE \n";
    printf("\n\n\t       VERTEX      DISTANCE\t        PATH");
    cout<<"\n\n\t\t"<<src<<"->"<<countp-1<<"\t      "<<dist[countp-1]<<"\t\t"<<src;

    path1=printPath(parent,countp-1,path);
    path[search(root,1)->key1][search(root,1)->key2]="0";
    cout<<"\n\n"<<endl;
    cout<<"\t\t\t\t   SOLUTION FOR THE MAZE \n\n";
    cout<<"\t\t\t\t\t";
    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(path[i][j]=="0")
                cout<<path[i][j]<<" ";
            else
                cout<<"  ";
        }
        cout<<endl;
        cout<<"\t\t\t\t\t";
    }
    return 0;
}

void dijkstra(int *graph, int src)
{

    int dist[countp];

    bool sptSet[countp];

    int parent[countp];

    for (int i = 0; i < countp; i++)
    {
        parent[0] = -1;
        dist[i] = inf;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < countp - 1; count++)
    {

        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < countp; v++)
        {
            if (!sptSet[v] && *((graph+u*countp) + v) &&
                dist[u] + *((graph+u*countp) + v) < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + *((graph+u*countp) + v);
            }
        }
    }
    if(dist[countp-1]!=inf)
        printSolution(dist, countp, parent);
    else
        cout<<"No solution for this maze\n\n";
}

int main()
{
    string word,new_word,file_name;                       //to read words from file
    int i,j,count=1,opt;
    i=j=0;
    int index1,index2;
    char option;
    int check=0;
    int size=0;
    cout<<"\n\n\n\n\n\n";
    cout<<"                       ***********************MAZE SOLVER***************************                           \n\n";
    cout<<"\n\n\tFILENAME TO READ(press d to load default matrix):   ";
    cin>>file_name;
    if(file_name.compare("d")==0)
        file_name="maze";

    file_name=file_name+".txt";
     ifstream file(file_name);

    getline(file,word);
    stringstream num(word);
    num>>N;                                      //to store

    string a[N][N];                             //to store matrix from file

    for(i=0;i<N;i++)
    {
        getline(file,word);
        std::istringstream iss(word);
        for(j=0;j<N;j++)
        {
            getline(iss,new_word ,' ');
            a[i][j]=new_word;
            if(new_word == "0")
            {
                root=insert(root,i,j,countp);
                countp++;
                if(i==0 || i==N-1)
                    check+=-1;
            }

        }
    }
    countp=countp-1;
    int b[countp][countp];                  //adjacent matrix

    for(i=0;i<countp;i++)
        for(j=0;j<countp;j++)
            b[i][j]=0;


    for(i=0;i<N-1;i++)
    {
        for(j=0;j<N;j++)
        {
            if(i!=0 && a[i-1][j]=="0" && a[i][j]=="0")
            {
                index1=key(root,i,j);
                index2=key(root,i-1,j);
                b[index1-1][index2-1]=1;
            }
            if(i!=N-1 && a[i+1][j]=="0" && a[i][j]=="0")
            {
                index1=key(root,i,j);
                index2=key(root,i+1,j);
                b[index1-1][index2-1]=1;
            }
            if(j!=0 && a[i][j-1]=="0" && a[i][j]=="0")
            {
                index1=key(root,i,j);
                index2=key(root,i,j-1);
                b[index1-1][index2-1]=1;
            }
            if(j!=N-1 && a[i][j+1]=="0" && a[i][j]=="0")
            {
                index1=key(root,i,j);
                index2=key(root,i,j+1);
                b[index1-1][index2-1]=1;
            }
        }
    }
    home:
    system("cls");
    opt=3;

    while(opt!=1 && opt!=2)
    {
        cout<<"\n\n\t\t\t\t\t\tOPTIONS\n\n\n\n";
        cout<<"\t\t\t1) PRINT ADJACENCY MATRIX OF THE GRAPH\n";
        cout<<"\t\t\t2) PRINT SOLUTION PATH FOR THE MAZE\n\n\t\t\t";
        cin>>opt;
    }
    system("cls");
    if(opt==1)
    {
        cout<<"\n\t\t\t\t\tADJACENCY MATRIX\n\n\n";
        cout<<"\t\t\t\t\t\t\t\tPRESS b TO MOVE BACK\n\n\n\t\t";
        for(i=0;i<countp;i++)
        {
            for(j=0;j<countp;j++)
            {
                cout<<b[i][j]<<"   ";                //print adj matrix
            }
            cout<<"\n\n\t\t";
        }
        cin>>option;
        if(option=='b')
         goto home;
        else
            exit(0);
    }
    else                                                  //print solution
    {
         if(check!=-2)
            cout<<"\t\t\tThere is no entry or exit for this maze\n";
         else
            dijkstra((int *)b,0);
    }
}
