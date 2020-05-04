#include "avltree.h"

NodeAvlServer *newNode(int key)
{
    NodeAvlServer *node = (NodeAvlServer *)checkedMalloc(sizeof(struct NodeAvl));
    
    node->id = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->client_desc = 0;
    return(node);
}

NodeAvlServer *singleRightRotate(struct NodeAvl *y)
{
    NodeAvlServer *temp = y->left;
    NodeAvlServer *T2 = temp->right;

    temp->right = y;
    y->left = T2;
    

    y->height = MAX(height(y->left), height(y->right)) + 1;
    temp->height = MAX(height(temp->right), height(temp->right)) + 1;

    return temp;
} 

NodeAvlServer *singleLeftRotate(struct NodeAvl *y)
{
    NodeAvlServer *temp = y->right;
    NodeAvlServer *T2 = temp->left;
    
    temp->left = y;
    y->right = T2;

    y->height = MAX(height(y->left), height(y->right)) + 1;
    temp->height = MAX(height(temp->right), height(temp->left)) + 1;

    return temp;
}

int getBalance(struct NodeAvl *y)
{
    if(y == NULL)
        return(0);
    return(height(y->left) - height(y->right));
}

int height(struct NodeAvl *y)
{
    if(y == NULL)
        return(0);

    return(y->height);
}

NodeAvlServer *insert(struct NodeAvl* node, int key)
{
    int balance;

    //1. Perform the normal BST insertion
    if(node == NULL)
        return(newNode(key));

    if(key < node->id)
        node->left = insert(node->left, key);
    else if(key > node->id)
        node->right = insert(node->right, key);
    else //equal keys are not allowed
        return node;

    //2. Update height of this ancestor node
    node->height = 1 + MAX(height(node->left), height(node->right));

    //3. Get the balance factor
    balance = getBalance(node);

    //4. If thie node is unbalanced

    //Left left case
    if(balance > 1 && key < node->left->id)
        return(singleRightRotate(node));

    //Right right case
    if(balance < -1 && key > node->left->id)
        return(singleLeftRotate(node));

    //Left right case
    if(balance > 1 && key > node->left->id)
    {
        node->left = singleLeftRotate(node->left);
        return(singleRightRotate(node));
    }

    //Right left case
    if(balance < -1 && key < node->right->id)
    {
        node->right = singleRightRotate(node->right);
        return(singleLeftRotate(node));
    }

    //return the unchanged node pointer
    return(node);
}

void preOrder(struct NodeAvl *root)
{
    if(root != NULL)
    {
        printf("%d", root->id);
        preOrder(root->left);
        preOrder(root->right);
    }
}
