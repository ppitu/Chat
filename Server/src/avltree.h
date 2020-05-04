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

} NodeAvlServer;

NodeAvlServer *newNode(int);
NodeAvlServer *singleRightRotate(struct NodeAvl *);
NodeAvlServer *singleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleRightRotate(struct NodeAvl *);
int height(struct NodeAvl *);
int getBalance(struct NodeAvl *);
NodeAvlServer *insert(struct NodeAvl *, int);
void preOrder(struct NodeAvl *);

#endif //AVLTREE_H_