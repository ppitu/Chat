#ifndef AVLTREE_H_
#define AVLTREE_H_

#include "lib.h"
#include "client_list.h"

typedef struct NodeAvl
{
    int id;
    struct NodeAvl *left;
    struct NodeAvl *right;
    //struct NodeAvl *up;
    int height;
    //int client_desc;
    ClientList *root_client_list;
    //char server_name[128];

} NodeAvlServer;

NodeAvlServer *AvlTreeNewNode(int, int);
NodeAvlServer *singleRightRotate(struct NodeAvl *);
NodeAvlServer *singleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleRightRotate(struct NodeAvl *);
int height(struct NodeAvl *);
int getBalance(struct NodeAvl *);
NodeAvlServer *AvlTreeInsert(struct NodeAvl *, int, int);
void AvlTreePreOrder(struct NodeAvl *);
NodeAvlServer *AvlTreeDeleteNode(struct NodeAvl *, int);
NodeAvlServer *AvlTreeMinValeNode(struct NodeAvl *);
int AvlTreeSize(struct NodeAvl*);
int *AvlTreeReturnIdArray(struct NodeAvl *);
void AvlTreeAddToArray(struct NodeAvl *, int[], int*);
bool AvlTreeContainId(struct NodeAvl *, int);
NodeAvlServer *AvlTreeFind(struct NodeAvl *, int);
void AvlTreeInsertUserToChat(struct NodeAvl *, char[], int, int);
int AvlTreeClientListSize(struct NodeAvl *, int );
int *AvlTreeClientListArrayDesc(struct NodeAvl *, int );
void AvlTreeRemoveClient(struct NodeAvl*, int, int);
void AvlTreeDestructor(struct NodeAvl*);
char *AvlTreeReturnNickName(struct NodeAvl *, int, int);

#endif //AVLTREE_H_