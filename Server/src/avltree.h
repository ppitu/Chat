#ifndef AVLTREE_H_
#define AVLTREE_H_

#include "lib.h"

typedef struct NodeAvl
{
    int id;
    struct NodeAvl *left;
    struct NodeAvl *right;
    //struct NodeAvl *up;
    int height;
    int client_desc;
    char server_name[128];

} NodeAvlServer;

NodeAvlServer *AvlTreeNewNode(int, int, char []);
NodeAvlServer *singleRightRotate(struct NodeAvl *);
NodeAvlServer *singleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleRightRotate(struct NodeAvl *);
int height(struct NodeAvl *);
int getBalance(struct NodeAvl *);
NodeAvlServer *AvlTreeInsert(struct NodeAvl *, int, int, char[]);
void AvlTreePreOrder(struct NodeAvl *);
NodeAvlServer *AvlTreeDeleteNode(struct NodeAvl *, int);
NodeAvlServer *AvlTreeMinValeNode(struct NodeAvl *);
int AvlTreeSize(struct NodeAvl*);
int *AvlTreeReturnIdArray(struct NodeAvl *);
void AvlTreeAddToArray(struct NodeAvl *, int[], int*);
bool AvlTreeContainId(struct NodeAvl *, int);

#endif //AVLTREE_H_