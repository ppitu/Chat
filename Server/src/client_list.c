#include "client_list.h"

ClientList *ClientListNewNode(int socket_desc, char *ip)
{
	ClientList *temp = (ClientList *)malloc(sizeof(ClientList));
	temp->data = socket_desc;
	temp->prev = NULL;
	temp->next = NULL;
	strncpy(temp->ip, ip, 16);
	strncpy(temp->name, "NULL", 5 );
	return(temp);
}

void deleteNode(struct ClientNode **root, int desc)
{
	ClientList 	*temp = *root;

	if(*root == NULL)
		return;

	/*if node to be deleted is head node*/
	if(temp->data == desc)
	{
		*root = temp->next;
	}

	while(temp != NULL && temp->data != desc)
		temp = temp->next;

	if(temp == NULL)
		return;

	/*if temp is NOT the last node*/
	if(temp->next != NULL)
	{
		temp->next->prev = temp->prev;
	}

	/*if temp is not first node*/
	if(temp->prev != NULL)
		temp->prev->next = temp->next;

	free(temp);
}

ClientList *lastElement(struct ClientNode **root)
{
	ClientList *temp = *root;

	if(temp->next == NULL)
		return temp;

	while(temp->next != NULL)
		temp = temp->next;

	return temp;
}

char *returnNickName(struct ClientNode **root, int desc)
{
	ClientList *temp = *root;

	if(temp->data == desc)
		return temp->name;

	while(temp != NULL && temp->data != desc)
		temp = temp->next;

	if(temp == NULL)
		return"";

	return temp->name;	
}

void setNickName(struct ClientNode **root, int desc, char name[])
{
	ClientList *temp = *root;

	if(temp->data == desc)
	{
		strcpy(name, temp->name);
	} else 
	{
		while(temp != NULL && temp->data != desc)
			temp = temp->next;

		if(temp == NULL)
		{

		}
		else
		{
			strncpy(temp->name, name, NAMELENGTH);
		}
	}
}


int ClientListSize(struct ClientNode* root)
{
	ClientList *temp = root;

	int size = 0;

	if(temp == NULL)
		return 0;

	while(temp != NULL)
	{
		size++;
		temp = temp->next;
	}

	return size;
}

int *ClientListDescArray(struct ClientNode*root)
{
	ClientList *temp = root;
	int i = 0;
	int *arr;

	arr= (int *)calloc(ClientListSize(temp), sizeof(int));

	if(temp == NULL)
	{
		*(arr + 0) = -1;
		return arr;
	}

	while(temp != NULL)
	{
		*(arr + i) = temp->data;
		i++;
		temp = temp->next;
	}

	return arr;
}