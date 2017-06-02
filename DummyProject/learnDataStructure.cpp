#include "learnDataStructure.h"
struct node
{
    int info;
    struct node *prev, *next;
};
struct node *STARTD = NULL;

struct ArrayStack
{
    int top;
    int cap;
    int *arrayStack;
};
struct ArrayStack *stackPtr = NULL;

struct ArrayQueue
{
    int frontQData,rearQData;
    int cap;
    int *arrayQueue;
};

struct BinaryTreeNode
{
    int data;
    struct BinaryTreeNode *leftChild;
    struct BinaryTreeNode *rightChild;
};

void preOrderTraversal(struct BinaryTreeNode *root)
{
    if(root)
    {
        cout<<" "<<root->data;
        preOrderTraversal(root->leftChild);
        preOrderTraversal(root->rightChild);
    }
}
void postOrderTraversal(struct BinaryTreeNode *root)
{
    if(root)
    {
        postOrderTraversal(root->leftChild);
        postOrderTraversal(root->rightChild);
        cout<<" "<<root->data;
    }
}
void inOrderTraversal(struct BinaryTreeNode *root)
{
    if(root)
    {
        inOrderTraversal(root->leftChild);
        cout<<" "<<root->data;
        inOrderTraversal(root->rightChild);
    }
}

struct BinaryTreeNode *FindInBST(struct BinaryTreeNode *root, int data)
{
    if(!root)
        return NULL;
    if(root->data == data)
        return root;
    else if (root->data > data)
        FindInBST(root->leftChild, data);
    else if (root->data < data)
        FindInBST(root->rightChild, data);
}
/*
struct BinaryTreeNode * DeleteBST (struct BinaryTreeNode *root, int data)
{
    struct BinaryTreeNode *temp;
    if(root == NULL)
        cout<<" \nBST is empty";
    else if(root->data < data)
        root->right = DeleteBST(root->right, data);
    else if(root->data > data)
        root->left = DeleteBST(root->left, data);
    else
    {
        if(root->left && root->right )//both child are presents
        {
            temp
        }
    }

}
*/

/*
void levelOrderTraversal(struct BinaryTreeNode *root)
{
    struct BinaryTreeNode *temp;
    struct ArrayQueue *Q = createQueue();
    if(!root)
        return;
    enQueue(Q,root);
    while(isEmptyQueue(Q))
    {
        temp = deQueue(Q);
        cout<<temp->data;
        if(temp->leftChild)
            enQueue(temp->leftChild);
        if(temp->rightChild)
            enQueue(temp->rightChild);
    }
    deleteQueue(Q);
}*/

struct ArrayQueue* createQueue(int capacity)
{
    struct ArrayQueue *Q = new ArrayQueue;
    Q->cap = capacity;
    Q->frontQData = -1;
    Q->rearQData = -1;
    Q->arrayQueue = new int[capacity];
    if(Q->arrayQueue == NULL)
        return NULL;
    else
        return Q;
}

int isEmptyQueue(struct ArrayQueue *temp)
{
    return (temp->frontQData == -1);
}

int isFullQueue (struct ArrayQueue *temp)
{
    return ((temp->rearQData+1)%temp->cap == temp->frontQData);
}

int QueueSize(struct ArrayQueue * temp)
{
    return ((temp->cap - temp->frontQData + temp->rearQData +1)% temp->cap);
}

void inQueue(struct ArrayQueue *temp, int value)
{
    if(isFullQueue(temp))
        cout<<"\n Queue is full OVERFLOW";
    else
    {
        temp->rearQData = (temp->rearQData+1)%temp->cap;
        temp->arrayQueue[temp->rearQData] = value;
        if(temp->frontQData == -1)
            temp->frontQData = temp->rearQData;
    }
}

void mujhePrintKaro()
{
    cout<<"\n I ma here 22222222222";
}

int showNaturalNumber (int x)
{
    int temp = 0;
    if(x==0)
    {
        cout<<"\n natural number end:";
        return 0;
    }
    else
    {
      cout<<"\n natural number : "<<x;
      temp = x + showNaturalNumber(x-1);
      return temp;
    }
}

void TOH(int n,char BEG,char AUX, char END)
{
    if(n>=1)
    {
        TOH(n-1, BEG, END, AUX);
        cout<<"\n"<<BEG<<" to "<<END;
        TOH(n-1, AUX, BEG, END);
    }
}

void addAsFirstNode()
{
    struct node *temp = NULL;
    temp = new node;
    cout<<"\n Enter node info ";
    cin>>temp->info;
    temp->prev = NULL;
    temp->next = NULL;
    if(STARTD != NULL)
    {
        temp->next = STARTD;
        STARTD->prev =  temp;
        STARTD = temp;
    }
    else
        STARTD = temp;
}

void deleteThePerticiularNode(int data)
{
    struct node *ptr = NULL;
    if (STARTD == NULL)
        cout<<"\nList is empty";
    else
        ptr = STARTD;
    while (ptr->next != NULL)
    {
       if(ptr->info == data)
            break;
       ptr = ptr->next;
    }

    if(ptr->info == data)
    {
        if(ptr == STARTD)
        {
            cout<<"\n This is the START node deleting it";
            STARTD = STARTD->next;
            delete ptr;
        }
        else
        {
            (ptr->prev)->next = ptr->next;
            cout<<"Deleting data "<<ptr->info;
            delete ptr;
        }
    }
    else
        cout<<"\n Data not found";
}

void showDoubleLinkList()
{
    struct node *ptr = STARTD;
    if(STARTD == NULL)
        cout<<"\nshowDoubleLinkList List is empty";
    else
    {
        cout<<"\nshowDoubleLinkList Showing double link list";
        while (ptr->next != NULL)
        {
            cout <<" "<<ptr->info;
            ptr = ptr->next;
        }
        cout <<" "<<ptr->info;
    }
}

void createStack(int capacity)
{
    stackPtr = new ArrayStack;
    stackPtr->top = -1;
    stackPtr->cap = capacity;
    stackPtr->arrayStack = new int[capacity];
}

void push_stack (int value)
{
    if( stackPtr->top == stackPtr->cap)
        cout<<"\n Stack is full";
    else
    {
        stackPtr->top = stackPtr->top +1;
        stackPtr->arrayStack[stackPtr->top] = value;
        cout<<" Add value "<<value;
    }
}
int pop_stack ()
{
    int stackValue =0;
    if( stackPtr->top < 0)
    {
        cout<<"\n Stack is empty";
        return -1;
    }
    else
    {
        stackValue = stackPtr->arrayStack[stackPtr->top];
        stackPtr->top = stackPtr->top - 1;
        return stackValue;
    }
}

bool binarySearch(int *ary, int length, int item)
{
    int l,u,m;
    l=0;
    u=length-1;
    m=(l+u)/2;
    while(l<=u)
    {
        if(ary[m] == item)
        {
            cout<<"\n Item found at index "<<m;
            break;
        }
        else if(ary[m] > item)
        {
            l = l;
            u = m-1;
            m = (l+u)/2;
        }
        else if(ary[m] < item)
        {
            l = m+1;
            u = u;
            m = (l+u)/2;
        }
    }
}


