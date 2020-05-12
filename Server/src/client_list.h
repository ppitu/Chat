#ifndef CILENTLIST_H_
#define CLIENTLIST_H_

#include "lib.h"

typedef struct ClientNode {
	int data;
	struct ClientNode* prev;
	struct ClientNode* next;
	char ip[16];
	char name[NAMELENGTH];
} ClientList;

ClientList *ClientListNewNode(int, char *);
void deleteNode(struct ClientNode **, int);
ClientList *lastElement(struct ClientNode **);
char *returnNickName(struct ClientNode **, int);
void setNickName(struct ClientNode** , int, char name[]);
int ClientListSize(struct ClientNode*);
int *ClientListDescArray(struct ClientNode*);
#endif //CLIENTLIST_H_
