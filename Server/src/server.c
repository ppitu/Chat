#include "lib.h"
#include "client_list.h"

ClientList *root = NULL;
ClientList *now = NULL;



void sendToAll(char tempbuffer[], int n, int desc)
{
	ClientList *temp = root->next;

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
}

void *doit(void * arg)
{
	ssize_t		n, err;
	char		recvLine[MAXLINE], sendLine[MAXLINE];
	int			connfd;


	connfd = *((int *)arg);

	free(arg);

	checkedPthread_detach(pthread_self());

	sprintf(recvLine, "%s join the chatroom.\n", returnNickName(&root, connfd));
	printf("%s\n", recvLine);
	sendToAll(recvLine, strlen(recvLine), connfd);

	while(1)
	{
		if((n = Readline(connfd, recvLine, MAXLINE)) == 0)
		{
			break;
		}
		recvLine[n] = '\0';

		sprintf(sendLine, "%s: %s", returnNickName(&root, connfd), recvLine);

		printf("%s", sendLine);

		sendToAll(sendLine, strlen(sendLine), connfd);
	}

	printf("close socket: %d\n", connfd);
	deleteNode(&root, connfd);

	checkedClose(connfd);
	return(NULL);
}

void exitServer(void)
{
	char str[80];

	while(1)
	{
		scanf("%s", str);

		if(str, "exit")
		{
		
		ClientList *temp = root->next;
		ClientList *temphelp;
		while(temp != NULL)
		{
			checkedClose(temp->data);
			printf("Close socket_desc: %d\n", temp->data);	
			temphelp = temp;
			temp = temp->next;
			free(temphelp);
		}	

		printf("Close socket_desc: %d\n", root->data);
		checkedClose(root->data);
		free(root);

		printf("Bye\n");
		exit(EXIT_SUCCESS);
		}
	
	}
}

int main(int argc, char **argv)
{

	int			socket_desc, *connect_desc, err, recv;
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

	root = newNode(socket_desc, "127.0.0.1");
	now = root;


	exitid = checkedCalloc(1, sizeof(pthread_t));
	checkedPthread_create(&exitid[0], NULL, &exitServer, NULL);

	while(1)
	{
		len = addrlen;
		connect_desc = checkedMalloc(sizeof(int));
		*connect_desc = checkedAccept(socket_desc, (struct sockaddr *)cliaddr, &len);

		checkedGetpeername(*connect_desc, (struct sockaddr*)cliaddr, &len);

		recv = checkedRead(*connect_desc, nickname, sizeof(nickname));


		nickname[recv] = '\0';

		now = lastElement(&root);
		
		ClientList *c = newNode(*connect_desc, inet_ntoa(cliaddr->sin_addr));
		c->prev = now;
		now->next = c;
		now = c;

		setNickName(&root, *connect_desc, nickname);

		checkedPthread_create(&thid, NULL, &doit, connect_desc);
	}
}
