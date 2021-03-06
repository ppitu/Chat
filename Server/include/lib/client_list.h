#ifndef CILENTLIST_H_
#define CLIENTLIST_H_

#include "lib.h"

typedef struct ClientNode {
	int socket_desc;
	struct ClientNode* prev;
	struct ClientNode* next;
	char ip[16];
	char nick_name[NAMELENGTH];
} ClientList;

ClientList *ClientListNewNode(int, char *);
void ClientListDeleteNode(struct ClientNode *, int);
ClientList *lastElement(struct ClientNode **);
char *ClientListReturnNickName(struct ClientNode *, int);
void setNickName(struct ClientNode** , int, char name[]);
int ClientListSize(struct ClientNode*);
int *ClientListDescArray(struct ClientNode*);
void ClientListRemoveClient(struct ClientNode*, int);
void ClientListDestructor(struct ClientNode*);
#endif //CLIENTLIST_H_
