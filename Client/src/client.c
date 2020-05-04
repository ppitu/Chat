#include "lib.h"

int 			socket_desc = 0;
volatile int 		flag = 0;
char 			nickname[NAMELENGTH];

void recv_msg()
{
	char	recvLine[MAXLINE];
	
	while(1)
	{
		if(flag == 1)
		{
			break;
		}
		int recv = read(socket_desc, recvLine, sizeof(recvLine));
		if(recv > 0 )
		{
			recvLine[recv] = '\0';
			printf("%s", recvLine);

		} else if (recv == 0)
		{
			flag = 1;
			break;
		} else {
		}
	}
}

void send_msg()
{
	char 	sendLine[1024];
	int 	end = 0;

	fflush(stdin);

	while(end == 0)
	{
		while(fgets(sendLine, MAXLINE, stdin) != NULL)
		{
			if(strcmp(sendLine, "exit\n") == 0)
			{
				flag = 1;
				end++;
				break;
			}

			checkedWrite(socket_desc, sendLine, strlen(sendLine));
		}
	}
}	

int main(int argc, char **argv)
{

	if(argc < 3)
	{
		printf("Nie podano portu lub\\i adresu IP\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter your nickname: ");
	scanf("%s", nickname);

	socket_desc = tcp_connect(argv[1], argv[2]);
	//socket_desc = tcp_connect("77.55.213.189", "2020");

	checkedWrite(socket_desc, nickname, strlen(nickname));

	pthread_t send_msg_thread;
	checkedPthread_create(&send_msg_thread, NULL, (void *)send_msg, NULL);

	pthread_t recv_msg_thread;
	checkedPthread_create(&recv_msg_thread, NULL, (void *)recv_msg, NULL);

	
	while(1)
	{
		if(flag == 1)
		{
			printf("Bye\n");
			break;
		}
	}


	checkedClose(socket_desc);
}
