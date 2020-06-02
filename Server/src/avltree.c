#include "avltree.h"

NodeAvlServer *AvlTreeNewNode(int key, int socket_desc, char *server_name)
{
    //NodeAvlServer *node = (NodeAvlServer *)checkedMalloc(sizeof(struct NodeAvl));
    NodeAvlServer *node = (NodeAvlServer *)checkedMalloc(sizeof(struct NodeAvl));
    
    node->id = key;
    strncpy(node->server_name, server_name, strlen(server_name));
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

NodeAvlServer *AvlTreeInsert(struct NodeAvl* node, int key, int socket_desc, char *name)
{
    int balance;

    //1. Perform the normal BST insertion
    if(node == NULL)
    {
        return(AvlTreeNewNode(key, socket_desc, name));
    }

    if(key < node->id)
        node->left = AvlTreeInsert(node->left, key, socket_desc, name);
    else if(key > node->id)
        node->right = AvlTreeInsert(node->right, key, socket_desc, name);
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

    AvlTreeIdToArray(node, arr, &i);

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

void AvlTreeIdToArray(struct NodeAvl *node, int arr[], int *i)
{
    if(node == NULL)
        return;

    AvlTreeIdToArray(node->left, arr, i);

    *(arr + *i) = node->id;
    ++*i;
    
    AvlTreeIdToArray(node->right, arr, i);

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

char **AvlTreeReturnServerNameArray(struct NodeAvl *node)
{
    char **arr;
    int avl_size = AvlTreeSize(node);
    int i;
    int j = 0;

    arr = (char **)malloc(avl_size * sizeof(char *));

    for(i = 0; i < avl_size; i++)
        arr[i] = (char *)malloc(NAMELENGTH * sizeof(char));

    //arr = (char *)malloc(2048 * sizeof(char));

    AvlTreeServerNameToArray(node, arr, &j);

    return arr;
}

void AvlTreeServerNameToArray(struct NodeAvl *node, char **arr, int *i)
{
    if(node == NULL)
        return;

    //strncat(arr, node->server_name, strlen(node->server_name));
    strcpy(arr[*i], node->server_name);
    ++*i;

    AvlTreeServerNameToArray(node->left, arr, i);
    AvlTreeServerNameToArray(node->right, arr, i);

}

int AvlTreeFindSmallestMissingIdValue(struct NodeAvl *node)
{
    int *arr;
    int i = 0;
    int tree_size = AvlTreeSize(node);
    int missing_value;

    if(tree_size == 0)
        return 0;


    arr= (int *)calloc(tree_size, sizeof(int));

    AvlTreeIdToArray(node, arr, &i);

    missing_value = ((tree_size + 1) * (tree_size + 2) /2);

    for(i = 0; i < tree_size; i++)
    {
        missing_value =- *(arr + i);
    }

    return(missing_value);
}