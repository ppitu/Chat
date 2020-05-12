#include "lib.h"
//#include "client_list.h"
#include "avltree.h"

int socket_desc;

//ClientList *root_client_list = NULL;
//ClientList *now = NULL;

NodeAvlServer *root_avl_tree = NULL;


/*void sendToAll(char tempbuffer[], int n, int *arr)
{
	ClientList *temp = root_client_list->next;

	while(temp != NULL)
	{
		if(temp->data == desc)
		{	
			temp = temp->next;
			continue;
		} 
		checkedWrite(temp->data, tempbuffer, n);
		temp = temp->next;
	}
}*/

void *doit(void * arg)
{
	ssize_t		n, err;
	char		recvLine[MAXLINE], sendLine[MAXLINE];
	int			connfd, avl_tree_size_doit;
	int			*arr;
	char 		len[10];
	int 		x;
	char		nickname[NAMELENGTH];
	int			recv;
	int 		*arrClient;
	int 		size;



	connfd = *((int *)arg);

	free(arg);

	checkedPthread_detach(pthread_self());

	recv = checkedRead(connfd, nickname, sizeof(nickname));


	nickname[recv] = '\0';

	/*now = lastElement(&root_client_list);
		
	ClientList *c = ClientListNewNode(connfd, "127");//inet_ntoa(cliaddr->sin_addr));
	c->prev = now;
	now->next = c;
	now = c;

	setNickName(&root_client_list, connfd, nickname);*/


	arr = AvlTreeReturnIdArray(root_avl_tree);
	avl_tree_size_doit = AvlTreeSize(root_avl_tree);

	if(avl_tree_size_doit > 0)
		printf("%d\n", *(arr + 0));

	//send len and avl-tree-id-list
	sprintf(len, "%d", avl_tree_size_doit);
	checkedWrite(connfd, len, sizeof(len));
	printf("Wyslalem len\n");

	checkedRead(connfd, recvLine, MAXLINE);
	printf("%s\n", recvLine);

	checkedWrite(connfd, arr, sizeof(int) * avl_tree_size_doit);
	//fwrite(arr, sizeof(int), sizeof(arr), connfd);
	printf("Send avl id array\n");
	//checkedWrite(connfd, "l", sizeof("l"));
	
	//n = checkedRead(connfd, recvLine, MAXLINE);

	//recvLine[n] = '\0';
	checkedRead(connfd, &x, sizeof(x));

	if(!AvlTreeContainId(root_avl_tree, x))
	{
		printf("Create new chatroom id: %d\n", x);
		root_avl_tree = AvlTreeInsert(root_avl_tree, x, socket_desc);
	}


	AvlTreeInsertUserToChat(root_avl_tree, nickname, connfd, x);

	size = AvlTreeClientListSize(root_avl_tree, x);
	arrClient = AvlTreeClientListArrayDesc(root_avl_tree, x);

	//sendToAll(recvLine, strlen(recvLine), connfd);

	for(int i = 0; i < size; i++)
	{
		write(*(arrClient + i), "hell", sizeof("hell"));
	}

	/*(sprintf(recvLine, "%s join the chatroom.\n", returnNickName(&root_client_list, connfd));
	printf("%s\n", recvLine);
	sendToAll(recvLine, strlen(recvLine), connfd);*/

	while(1)
	{
		if((n = Readline(connfd, recvLine, MAXLINE)) == 0)
		{
			break;
		}
		recvLine[n] = '\0';

		//sprintf(sendLine, "%s: %s", returnNickName(&root_client_list, connfd), recvLine);

		printf("%s", recvLine);

		size = AvlTreeClientListSize(root_avl_tree, x);
		arrClient = AvlTreeClientListArrayDesc(root_avl_tree, x);

		//sendToAll(recvLine, strlen(recvLine), connfd);

		for(int i = 0; i < size; i++)
		{
			write(*(arrClient + i), "hell", sizeof("hell"));
		}

		//sendToAll(sendLine, strlen(sendLine), connfd);
	}
/*
	free(arr);

	printf("close socket: %d\n", connfd);
	deleteNode(&root_client_list, connfd);

	checkedClose(connfd);
	*/
	printf("lol\n");
	return(NULL);
}

/*void exitServer(void)
{
	char str[80];

	while(1)
	{
		scanf("%s", str);

		if(strcmp(str, "user") == 0)
		{
			AvlTreePreOrder(root_avl_tree);
			printf("\n");
		}

		if(strcmp(str, "exit") == 0)
		{
		
		ClientList *temp = root_client_list->next;
		ClientList *temphelp;
		while(temp != NULL)
		{
			checkedClose(temp->data);
			printf("Close socket_desc: %d\n", temp->data);	
			temphelp = temp;
			temp = temp->next;
			free(temphelp);
		}	

		printf("Close socket_desc: %d\n", root_client_list->data);
		checkedClose(root_client_list->data);
		free(root_client_list);

		printf("Bye\n");
		exit(EXIT_SUCCESS);
		}
	
	}
}*/

int main(int argc, char **argv)
{

	int			*connect_desc, err, recv;
	socklen_t		addrlen, len;
	struct sockaddr_in	*cliaddr;
	pthread_t		thid, *exitid;
	char			nickname[NAMELENGTH];

	if(argc == 2)
		socket_desc = tcp_listen(NULL, argv[1], &addrlen);
	else if(argc == 3)
		socket_desc = tcp_listen(argv[1], argv[2], &addrlen);
	else
	{
		printf("error <service or port>\n");
		exit(EXIT_FAILURE);
	}

	if(socket_desc == -1)
	{
		printf("socket error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	cliaddr = checkedMalloc(addrlen);

	//root_client_list = ClientListNewNode(socket_desc, "127.0.0.1");
	//now = root_client_list;


	//exitid = checkedCalloc(1, sizeof(pthread_t));
	//checkedPthread_create(&exitid[0], NULL, (void*)&exitServer, NULL);

	while(1)
	{
		len = addrlen;
		connect_desc = checkedMalloc(sizeof(int));
		*connect_desc = checkedAccept(socket_desc, (struct sockaddr *)cliaddr, &len);

		checkedGetpeername(*connect_desc, (struct sockaddr*)cliaddr, &len);

		//recv nickname
		/*recv = checkedRead(*connect_desc, nickname, sizeof(nickname));


		nickname[recv] = '\0';

		now = lastElement(&root_client_list);
		
		ClientList *c = ClientListNewNode(*connect_desc, inet_ntoa(cliaddr->sin_addr));
		c->prev = now;
		now->next = c;
		now = c;

		setNickName(&root_client_list, *connect_desc, nickname);*/

		checkedPthread_create(&thid, NULL, &doit, connect_desc);
	}
}
