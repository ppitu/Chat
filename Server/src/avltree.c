#include "avltree.h"

NodeAvlServer *AvlTreeNewNode(int key, int socket_desc)
{
    //NodeAvlServer *node = (NodeAvlServer *)checkedMalloc(sizeof(struct NodeAvl));
    NodeAvlServer *node = (NodeAvlServer *)malloc(sizeof(struct NodeAvl));
    
    node->id = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    node->root_client_list = ClientListNewNode(socket_desc, "127.0.0.1");
    //node->client_desc = desc;
    //snprintf(node->server_name, 128, server_name);
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

NodeAvlServer *AvlTreeInsert(struct NodeAvl* node, int key, int socket_desc)
{
    int balance;

    //1. Perform the normal BST insertion
    if(node == NULL)
    {
        return(AvlTreeNewNode(key, socket_desc));
    }

    if(key < node->id)
        node->left = AvlTreeInsert(node->left, key, socket_desc);
    else if(key > node->id)
        node->right = AvlTreeInsert(node->right, key, socket_desc);
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
        printf("%d\n", node->id);
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

int *AvlTreeReturnIdArray(struct NodeAvl *node)
{
    int *arr;
    int i = 0;

    arr= (int *)calloc(AvlTreeSize(node), sizeof(int));// * AvlTreeSize(node));

    AvlTreeAddToArray(node, arr, &i);

    return(arr);
}

int AvlTreeSize(struct NodeAvl * node)
{
    if(node == NULL)
        return(0);
    else
    {
        return(AvlTreeSize(node->left) + 1 + AvlTreeSize(node->right));
    }
    
}

void AvlTreeAddToArray(struct NodeAvl *node, int arr[], int *i)
{
    if(node == NULL)
        return;

    *(arr + *i) = node->id;
    ++*i;
    AvlTreeAddToArray(node->left, arr, i);
    AvlTreeAddToArray(node->right, arr, i);

}

bool AvlTreeContainId(struct NodeAvl *node, int id)
{
    if(node == NULL)
        return false;
    else{
        if(id == node->id)
            return true;
        else if(id < node->id)
            AvlTreeContainId(node->left, id);
        else if(id > node->id)
            AvlTreeContainId(node->right, id);
    }
}

NodeAvlServer *AvlTreeFind(struct NodeAvl * node, int id)
{
    NodeAvlServer *current = node;

    while(current->id != id)
    {
        if(current != NULL)
        {
            if(current->id > id)
                current = current->left;
            else
            {
                current = current->right;
            }

            if(current == NULL)
                return NULL;
            
        }

    }

    return current;
}

void AvlTreeInsertUserToChat(struct NodeAvl *node, char nickname[], int desc, int id)
{
    NodeAvlServer *current = NULL;

    current = AvlTreeFind(node, id);

    if(current == NULL)
        return;

    ClientList *now = NULL;
    now = lastElement(&current->root_client_list);

    ClientList *c = ClientListNewNode(desc, "127");
    c->prev = now;
	now->next = c;
	now = c;

    setNickName(&current->root_client_list, desc, nickname);
    
}

int AvlTreeClientListSize(struct NodeAvl *node, int id)
{
    NodeAvlServer *current = NULL;

    current = AvlTreeFind(node, id);

    if(current == NULL)
        return 0;

    return(ClientListSize(current->root_client_list));
}

int *AvlTreeClientListArrayDesc(struct NodeAvl *node, int id)
{
    int *arr;

    NodeAvlServer *current = NULL;

    current = AvlTreeFind(node, id);

    if(current == NULL)
    {
        *(arr + 0) = -1;
        return arr;
    }

    return(ClientListDescArray(current->root_client_list));


}

void AvlTreeRemoveClient(struct NodeAvl *node, int client, int id)
{
    NodeAvlServer *current = NULL;

    current = AvlTreeFind(node, id);

    if(current == NULL)
        return;

    ClientListDeleteNode(current->root_client_list, client);
}

void AvlTreeDestructor(struct NodeAvl *node)
{
    if(node == NULL)
        return;

    NodeAvlServer *temp = node;

    if(temp->left != NULL)
        AvlTreeDestructor(temp->left);
    if(temp->right != NULL)
        AvlTreeDestructor(temp->right);
    
    ClientListDestructor(temp->root_client_list);

    free(temp);
    return;

}

char *AvlTreeReturnNickName(struct NodeAvl *node, int id, int desc)
{
    NodeAvlServer *current = NULL;

    current = AvlTreeFind(node, id);

    return(ClientListReturnNickName(current->root_client_list, desc));
}
