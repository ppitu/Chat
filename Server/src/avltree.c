#include "avltree.h"

NodeAvlServer *AvlTreeNewNode(int key, char server_name[])
{
    //NodeAvlServer *node = (NodeAvlServer *)checkedMalloc(sizeof(struct NodeAvl));
    NodeAvlServer *node = (NodeAvlServer *)malloc(sizeof(struct NodeAvl));
    
    node->id = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    snprintf(node->server_name, 128, server_name);
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

    return(temp);
} 

NodeAvlServer *singleLeftRotate(struct NodeAvl *y)
{
    NodeAvlServer *temp = y->right;
    NodeAvlServer *T2 = temp->left;
    
    temp->left = y;
    y->right = T2;

    y->height = MAX(height(y->left), height(y->right)) + 1;
    temp->height = MAX(height(temp->right), height(temp->left)) + 1;

    return(temp);
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

NodeAvlServer *AvlTreeInsert(struct NodeAvl* node, int key, char server_name[])
{
    int balance;

    //1. Perform the normal BST insertion
    if(node == NULL)
        return(AvlTreeNewNode(key, server_name));

    if(key < node->id)
        node->left = AvlTreeInsert(node->left, key, server_name);
    else if(key > node->id)
        node->right = AvlTreeInsert(node->right, key, server_name);
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
    if(balance < -1 && key > node->right->id)
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

void AvlTreePreOrder(struct NodeAvl *node)
{
    if(node != NULL)
    {
        printf("%d %s\n", node->id, node->server_name);
        AvlTreePreOrder(node->left);
        AvlTreePreOrder(node->right);
    }
}

NodeAvlServer *AvlTreeDeleteNode(struct NodeAvl *node, int key)
{
    //1. perform standard bst delete
    if(node = NULL)
        return node;

    if(key < node->id)
        node->left = AvlTreeDeleteNode(node->left, key);
    else if(key > node->id)
        node->right = AvlTreeDeleteNode(node->right, key);
    else
    {
        //node with only one child or no child
        if((node->left == NULL) || (node->right) == NULL)
        {
            NodeAvlServer *temp = node->left ? node->left : node->right;

            //no child case
            if(temp == NULL)
            {
                temp = node;
                node = NULL;
            }
            else //one child case
                *node = *temp;

            free(temp);
        } else
        {
            //node with two children
            NodeAvlServer *temp = AvlTreeMinValeNode(node->right);

            node->id = temp->id;

            node->right = AvlTreeDeleteNode(node->right, temp->id);
        }
        
    }

    //if the tree had only one node then return
    if(node = NULL)
        return(node);
    
    

}

NodeAvlServer *AvlTreeMinValeNode(struct NodeAvl * node)
{
    NodeAvlServer *current = node;

    while(current->left != NULL)
        current = current->left;

    return current;
}