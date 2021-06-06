#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

struct Node
{
    int key1,key2,index;
    Node *left;
    Node *right;
    int height;
};

// A utility function to get maximum
// of two integers
int max(int a, int b);

// A utility function to get the
// height of the tree
int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum
// of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Helper function that allocates a
   new node with the given key and
   NULL left and right pointers. */
Node* newNode(int a,int b,int k)
{
    Node* node = new Node();
    node->key1 = a;
    node->key2=b;
    node->index=k;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // new node is initially
                      // added at leaf
    return(node);
}

// A utility function to right
// rotate subtree rooted with y
// See the diagram given above.
Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left),
                    height(y->right)) + 1;
    x->height = max(height(x->left),
                    height(x->right)) + 1;

    // Return new root
    return x;
}

// A utility function to left
// rotate subtree rooted with x
// See the diagram given above.
Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left),
                    height(x->right)) + 1;
    y->height = max(height(y->left),
                    height(y->right)) + 1;

    // Return new root
    return y;
}

// Get Balance factor of node N
int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Recursive function to insert a key
// in the subtree rooted with node and
// returns the new root of the subtree.
Node* insert(Node* node, int a,int b,int k)
{
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
        return(newNode(a,b,k));

    if (a< node->key1)
        node->left = insert(node->left, a,b,k);
    else if (a > node->key1)
        node->right = insert(node->right, a,b,k);
    else if(a==node->key1)
    {
        if(b<node->key2)
            node->left=insert(node->left,a,b,k);
        else if(b> node->key2)
        {
            node->right=insert(node->right,a,b,k);
        }
        else
            return node;
    }



    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                        height(node->right));

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && (a < node->left->key1 || b<node->left->key2))
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && (a > node->right->key1 || b>node->right->key2))
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && (a > node->left->key1 || b > node->left->key2))
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && (a < node->right->key1 || b < node->right->key2 ))
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

// A utility function to print preorder
// traversal of the tree.
// The function also prints height
// of every node
Node * minValueNode(Node* node)
{
    Node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}
void preOrder(Node *root)
{
    if(root != NULL)
    {
        preOrder(root->left);
        cout << root->key1 << ":"<<root->key2<<":"<<root->index<<"\n";
       preOrder(root->right);
    }
}

struct Node* search(struct Node* root, int key1,int key2)
{
    if (root == NULL || (root->key1 == key1 && root->key2 ==key2))
       return root;

    if (root->key1 < key1)
       return search(root->right, key1,key2);
    else if(root->key1 > key1)
        return search(root->left, key1,key2);
    else if(root->key1==key1)
    {
         if (root->key2 < key2)
       return search(root->right, key1,key2);
    else
        return search(root->left, key1,key2);
    }
}
Node* check_adjacency(Node* n)
{
	Node* found=NULL;
	 found=search(n,n->key1-1,n->key2);
	 if(found!=NULL)
     {
         return found;
     }
     else
     {
         found=search(n,n->key1,n->key2-1);
         if(found!=NULL)
             return found;
         else
         {
             found=search(n,n->key1+1,n->key2);
             if(found!=NULL)
                return found;
             else
            {
                    found=search(n,n->key1,n->key2+1);
                    if(found!=NULL)
                        return found;
                    else
                        return NULL;
             }
         }
     }
}

int* create_adj_matrix( Node* n, int k, int *adj_matrix )
{
	Node* adj=NULL;
	if(n != NULL)
    {
        create_adj_matrix(n->left,k,adj_matrix);
        adj=check_adjacency(n);
        if(adj!=NULL)
        {
            *((adj_matrix+(n->index-1)*k)+adj->index-1)=1;
        }
       create_adj_matrix(n->right,k,adj_matrix);
    }
    return adj_matrix;

}

int main()
{
    struct Node* root=NULL;
    struct Node* found=NULL;
    string word,new_word;
    int i,j,k;

    i=j=0;
    k=1;

    ifstream file;
    file.open("ads.txt",ios::out);

    while(getline(file,word))
    {
        j=0;
        std::istringstream iss(word);
        while(getline(iss,new_word ,' '))
        {
            if(new_word == "0")
            {
                root=insert(root,i,j,k);
                k++;

            }
            j++;
        }
        i++;
    }
     int *adj_matrix=NULL;
    //preOrder(root);
    adj_matrix=create_adj_matrix(root, k ,(int*)adj_matrix);
    for(int i=0;i<k;i++)
    {
        for(int j=0;j<k;j++)
            cout<<*((adj_matrix+i*k)+j)<<"\n";

    }
}

