#include "lib.h"
#include "avltree.h"

#include    <json-c/json.h>

int socket_desc;



NodeAvlServer *root_avl_tree = NULL;

//the function sends a message to everyone in the chat
void sendToAll(char tempbuffer[], int *arr, int size, int desc)
{
	int i;

	for(i = 1; i < size; i++)
		{
			if(*(arr + i) == desc)
				continue;
			checkedWrite(*(arr + i), tempbuffer, strlen(tempbuffer));
		}
}

void *doit(void * arg)
{
	ssize_t		n, err;
	char		recvLine[MAXLINE], sendLine[MAXLINE];
	int			connfd, avl_tree_size_doit;
	int			*arr;
	char 		len[10];
	int 		id; //server id on the avl tree
	char		nickname[NAMELENGTH];
	int			recv;
	int 		*arrClient;
	int 		size;
	char 		**server_name;
	char		name_server[NAMELENGTH];

	connfd = *((int *)arg);

	free(arg);

	checkedPthread_detach(pthread_self());

	recv = checkedRead(connfd, nickname, sizeof(nickname));


	nickname[recv] = '\0';


	if(avl_tree_size_doit > 0)
		printf("%d\n", *(arr + 0));


	SendChatRoomList(connfd, root_avl_tree);


	checkedRead(connfd, &id, sizeof(id));

	checkedRead(connfd, name_server, sizeof(name_server));

	//if the avl tree does not contain chat on the given id create a new chat
	if(!AvlTreeContainId(root_avl_tree, id))
	{
		printf("Create new chatroom id: %d and name: %s\n", id, name_server);
		root_avl_tree = AvlTreeInsert(root_avl_tree, id, socket_desc, name_server);
	}


	AvlTreeInsertUserToChat(root_avl_tree, nickname, connfd, id);

	//download the number of users and their desc
	size = AvlTreeClientListSize(root_avl_tree, id);
	arrClient = AvlTreeClientListArrayDesc(root_avl_tree, id);

	sprintf(recvLine, "%s join the chatroom.\n", AvlTreeReturnNickName(root_avl_tree, id, connfd));
	printf("%s", recvLine);
	sendToAll(recvLine, arrClient, size, connfd);

	while(1)
	{
		if((n = Readline(connfd, recvLine, MAXLINE)) == 0)
		{
			break;
		}
		recvLine[n] = '\0';

		//sprintf(sendLine, "%s: %s", AvlTreeReturnNickName(root_avl_tree, x, connfd), recvLine);

		printf("%s", recvLine);

		size = AvlTreeClientListSize(root_avl_tree, id);
		arrClient = AvlTreeClientListArrayDesc(root_avl_tree, id);

		sendToAll(recvLine, arrClient, size, connfd);

	}

	sprintf(recvLine, "%s leaves the chat\n", AvlTreeReturnNickName(root_avl_tree, id, connfd));
	sendToAll(recvLine, arrClient, size, connfd);

	free(arrClient);
	
	printf("close socket: %d\n", connfd);
	AvlTreeRemoveClient(root_avl_tree, connfd, id);

	checkedClose(connfd);

	return(NULL);
}

void exitServer(void)
{
	char str[80];

	while(1)
	{
		scanf("%s", str);

		/*if(strcmp(str, "user") == 0)
		{
			AvlTreePreOrder(root_avl_tree);
			printf("\n");
		}*/

		if(strcmp(str, "exit") == 0)
		{

			AvlTreeDestructor(root_avl_tree);

			printf("Bye\n");
			exit(EXIT_SUCCESS);
		}
	
	}
}

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


	//server command support
	exitid = checkedCalloc(1, sizeof(pthread_t));
	checkedPthread_create(&exitid[0], NULL, (void*)&exitServer, NULL);

	while(1)
	{
		//answer call and start user thread
		len = addrlen;
		connect_desc = checkedMalloc(sizeof(int));
		*connect_desc = checkedAccept(socket_desc, (struct sockaddr *)cliaddr, &len);

		checkedGetpeername(*connect_desc, (struct sockaddr*)cliaddr, &len);

		checkedPthread_create(&thid, NULL, &doit, connect_desc);
	}
}
