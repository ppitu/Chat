#include "lib.h"

#include    <json-c/json.h>

int 			socket_desc = 0;
volatile int 		flag = 0;
char 			nickname[NAMELENGTH];


void recv_msg()
{
	char	recvLine[MAXLINE];
	int recv;
	
	while(1)
	{
		if(flag == 1)
		{
			break;
		}
		recv = checkedRead(socket_desc, recvLine, sizeof(recvLine));
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
	char 	server_name[128];
	int 	*arr;
	int		len, i;
	int 	id;
	int 	n;
	char 	json_buffer[1024];
	size_t 	json_array_size;
	struct json_object *paresd_json;
	struct json_object *jarr;
	struct json_object *jarrhelp;
	struct json_object *jarrstring;
	struct json_object *jarrstringhelp;
	


	if(argc < 3)
	{
		printf("Nie podano portu lub\\i adresu IP\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter your nickname: ");
	scanf("%s", nickname);

	socket_desc = tcp_connect(argv[1], argv[2]);
	//socket_desc = tcp_connect("77.55.213.189", "2020");

	//send nickname
	checkedWrite(socket_desc, nickname, strlen(nickname));

	//read json 

	checkedRead(socket_desc, json_buffer, sizeof(json_buffer));

	paresd_json = json_tokener_parse(json_buffer);

	json_object_object_get_ex(paresd_json, "Array", &jarr);
	json_object_object_get_ex(paresd_json, "Server Name", &jarrstring);
	//json_object_object_get_ex(paresd_json, "Size", &jintsize);

	json_array_size = json_object_array_length(jarr);

	if(json_array_size  == 0)
		printf("none\n");
	else
	{
		for(i = 0; i < json_array_size; i++)
	{
		jarrhelp = json_object_array_get_idx(jarr, i);
		jarrstringhelp = json_object_array_get_idx(jarrstring, i);
		printf("%d: %s\n", json_object_get_int(jarrhelp), json_object_get_string(jarrstringhelp));
	}
	}
	

	printf("Enter server name: ");
	scanf("%s", server_name);

	printf("Enter id: ");
	scanf("%d", &id);

	//checkedWrite(socket_desc, server_name, strlen(server_name));
	checkedWrite(socket_desc, &id, sizeof(id));

	checkedWrite(socket_desc, server_name, strlen(server_name));

	pthread_t send_msg_thread;
	checkedPthread_create(&send_msg_thread, NULL, (void *)send_msg, NULL);

	pthread_t recv_msg_thread;
	checkedPthread_create(&recv_msg_thread, NULL, (void *)recv_msg, NULL);


	pthread_join(send_msg_thread, NULL);


	if(flag == 1)
	{
		printf("Bye\n");
	}




	checkedClose(socket_desc);
}
