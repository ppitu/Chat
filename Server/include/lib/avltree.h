#ifndef AVLTREE_H_
#define AVLTREE_H_

#include "lib.h"
#include "client_list.h"

typedef struct NodeAvl
{
    int id;
    char server_name[NAMELENGTH];
    struct NodeAvl *left;
    struct NodeAvl *right;
    int height;
    bool private;
    ClientList *root_client_list;

} NodeAvlServer;

NodeAvlServer *AvlTreeNewNode(int, int, char *, bool);
NodeAvlServer *singleRightRotate(struct NodeAvl *);
NodeAvlServer *singleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleLeftRotate(struct NodeAvl *);
//NodeAvlServer *doubleRightRotate(struct NodeAvl *);
int height(struct NodeAvl *);
int getBalance(struct NodeAvl *);
NodeAvlServer *AvlTreeInsert(struct NodeAvl *, int, int, char *, bool);
void AvlTreePreOrder(struct NodeAvl *);
NodeAvlServer *AvlTreeDeleteNode(struct NodeAvl *, int);
NodeAvlServer *AvlTreeMinValeNode(struct NodeAvl *);
int AvlTreeSize(struct NodeAvl*);
int *AvlTreeReturnIdArray(struct NodeAvl *);
void AvlTreeIdToArray(struct NodeAvl *, int[], int*);
bool AvlTreeContainId(struct NodeAvl *, int);
bool AvlTreeContainServerName(struct NodeAvl *, char *, int );
NodeAvlServer *AvlTreeFind(struct NodeAvl *, int);
void AvlTreeInsertUserToChat(struct NodeAvl *, char[], int, int);
int AvlTreeClientListSize(struct NodeAvl *, int );
int *AvlTreeClientListArrayDesc(struct NodeAvl *, int );
void AvlTreeRemoveClient(struct NodeAvl*, int, int);
void AvlTreeDestructor(struct NodeAvl*);
char *AvlTreeReturnNickName(struct NodeAvl *, int, int);
char **AvlTreeReturnServerNameArray(struct NodeAvl *);
void AvlTreeServerNameToArray(struct NodeAvl *, char **, int *);
int AvlTreeFindSmallestMissingIdValue(struct NodeAvl*);
int AvlTreeSmallestMissingId(int[], int, int);


#endif //AVLTREE_H_