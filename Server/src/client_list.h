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

ClientList *newNode(int, char *);
void deleteNode(struct ClientNode **, int);
ClientList *lastElement(struct ClientNode **);
char *returnNickName(struct ClientNode **, int);
void setNickName(struct ClientNode** , int, char name[]);
#endif //CLIENTLIST_H_
