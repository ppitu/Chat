#include "lib.h"
#include "avltree.h"

#include   <json-c/json.h>
#include    <mysql/mysql.h>

int socket_desc;

MYSQL *con;

char *server = "localhost";
char *user = "root";
char pass[128];
char *database = "";

char query[254] = {0};

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
	char		nickname[NAMELENGTH] = "lol";
	int			recv;
	int 		*arrClient;
	int 		size;
	char 		**server_name;
	int			choice;
	char		name_server[NAMELENGTH];
	int 		contain;
	int 		pom = 0;
	int			value = -1;
	

	connfd = *((int *)arg);

	free(arg);

	checkedPthread_detach(pthread_self());

	value = AvlTreeFindSmallestMissingIdValue(root_avl_tree);
	printf("Value %d\n", value);
	//recive nickname
	//recv = checkedRead(connfd, nickname, sizeof(nickname));


	//nickname[recv] = '\0';

	loggingRegisterUser(connfd, con);

	checkedRead(connfd, recvLine, sizeof(recvLine));

	if(avl_tree_size_doit > 0)
		printf("%d\n", *(arr + 0));


	SendChatRoomList(connfd, root_avl_tree);

	id = CreateJoinServerUserChoice(&root_avl_tree, connfd,  socket_desc);


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
			mysql_close(con);
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

	printf("MySQL client version: %s\n", mysql_get_client_info());

	con = mysql_init(NULL);

	printf("Database password: ");
	scanf("%s", pass);

	if(!mysql_real_connect(con, server, user, pass, database, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(con));
	}

	mysql_query(con, "use chat");

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
