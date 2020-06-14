#include "lib.h"

#include    <json-c/json.h>
#include <mysql/mysql.h>

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
	char 	password[NAMELENGTH];
	int 	*arr;
	int		*idarray;
	int		len, i, d, q;
	int 	id;
	int 	contain = 0;
	int 	n;
	char 	json_buffer[1024];
	int		choice;
	int 	choicelog;
	int confirm;
	int recv_int;
	size_t 	json_array_size;
	struct json_object *paresd_json;
	struct json_object *jarr;
	struct json_object *jarrhelp;
	struct json_object *jarrstring;
	struct json_object *jarrstringhelp;
	struct json_object *jobj;
	struct json_object *jobjlog;
	struct json_object *jserver_name;
	struct json_object *jnickname;
	struct json_object *jpassword;
	struct json_object *jlogarray;
	struct json_object *jint;
	


	if(argc < 3)
	{
		printf("Nie podano portu lub\\i adresu IP\n");
		exit(EXIT_FAILURE);
	}

	socket_desc = tcp_connect(argv[1], argv[2]);
	//socket_desc = tcp_connect("77.55.213.189", "2020");

	sprawdzenie:

	printf("[1] Sign in or [2] registration or [3] as a guest:");
	scanf("%d", &choicelog);

	jobjlog = json_object_new_object();
	jlogarray = json_object_new_array();

	if(choicelog == 1)
	{
		printf("Enter nickname: ");
		scanf("%s", nickname);
		printf("Enter password: ");
		scanf("%s", password);

		jnickname = json_object_new_string(nickname);
		jpassword = json_object_new_string(password);

		json_object_array_add(jlogarray, jnickname);
		json_object_array_add(jlogarray, jpassword);

		json_object_object_add(jobjlog, "Logging", jlogarray);

		printf("The josn log %s\n", json_object_to_json_string(jobjlog));
	} else if(choicelog == 2)
	{
		printf("Enter nickname: ");
		scanf("%s", nickname);
		printf("Enter password: ");
		scanf("%s", password);

		jnickname = json_object_new_string(nickname);
		jpassword = json_object_new_string(password);

		json_object_array_add(jlogarray, jnickname);
		json_object_array_add(jlogarray, jpassword);

		json_object_object_add(jobjlog, "Register", jlogarray);

		printf("The josn reg %s\n", json_object_to_json_string(jobjlog));


	}else if(choicelog == 3)
	{
		printf("Enter your nickname: ");
		scanf("%s", nickname);

		jnickname = json_object_new_string(nickname);

		json_object_array_add(jlogarray, jnickname);

		json_object_object_add(jobjlog, "Guest", jlogarray);

		printf("The josn log %s\n", json_object_to_json_string(jobjlog));
	} else {

	}

	
	
	//send login json
	checkedWrite(socket_desc, json_object_to_json_string(jobjlog), strlen(json_object_to_json_string(jobjlog)));
	puts("lolol");
	checkedRead(socket_desc, json_buffer, sizeof(json_buffer));
	puts("lolol");
	//confirm = ntohl(recv_int);
	paresd_json = json_tokener_parse(json_buffer);
	json_object_object_get_ex(paresd_json, "Confirm", &jint);
	confirm = json_object_get_int(jint);

	printf("%ld\n", sizeof(confirm));
	printf("%d\n", confirm);

	if(confirm == 1)
		goto sprawdzenie;

	checkedWrite(socket_desc, "Lol\n", sizeof("Lol\n"));

	//send nickname
	//checkedWrite(socket_desc, nickname, strlen(nickname));

	//read json 
	puts("looooasdlo");
	checkedRead(socket_desc, json_buffer, sizeof(json_buffer));

	paresd_json = json_tokener_parse(json_buffer);

	json_object_object_get_ex(paresd_json, "Array", &jarr);
	json_object_object_get_ex(paresd_json, "Server Name", &jarrstring);
	//json_object_object_get_ex(paresd_json, "Size", &jintsize);

	json_array_size = json_object_array_length(jarr);

	idarray = (int *)malloc(json_array_size * sizeof(int));

	printf("Server list:\n");
	printf("Nr \tId \tServer Name\n");

	if(json_array_size  == 0)
		printf("none\n");
	else
	{
		for(i = 0; i < json_array_size; i++)
	{
		jarrhelp = json_object_array_get_idx(jarr, i);
		*(idarray + i) = json_object_get_int(jarrhelp);
		jarrstringhelp = json_object_array_get_idx(jarrstring, i);
		printf("%d.\t%d\t%s\n", i + 1, json_object_get_int(jarrhelp), json_object_get_string(jarrstringhelp));
	}
	}
	
	printf("If you want to join select 1 if you want to create server select 2:\n");
	scanf("%d", &choice);

	jobj = json_object_new_object();

	//send choice
	checkedWrite(socket_desc, &choice, sizeof(choice));

	if(choice == 1)
	{	
		while(contain == 0)
		{
			printf("Enter server id or -1 to exit:\n");
			scanf("%d", &id);

			if(id == -1)
				goto koniec;

			checkedWrite(socket_desc, &id, sizeof(id));
			checkedRead(socket_desc, &contain, sizeof(contain));

		}
		
	} else if(choice == 2)
	{
		while(contain == 0)
		{
			printf("Enter server name or -1 to exit:\n");
			scanf("%s", server_name);

			if(server_name == "-1")
				goto koniec;

			jserver_name = json_object_new_string(server_name);
			json_object_object_add(jobj, "Server Name", jserver_name);

			printf("The josn %s\n", json_object_to_json_string(jobj));

			checkedWrite(socket_desc, json_object_to_json_string(jobj), strlen(json_object_to_json_string(jobj)));
			checkedRead(socket_desc, &contain, sizeof(contain));
		}
	} else 
	{
		goto koniec;
	}

	pthread_t send_msg_thread;
	checkedPthread_create(&send_msg_thread, NULL, (void *)send_msg, NULL);

	pthread_t recv_msg_thread;
	checkedPthread_create(&recv_msg_thread, NULL, (void *)recv_msg, NULL);


	pthread_join(send_msg_thread, NULL);

	koniec:

	if(flag == 1)
	{
		printf("Bye\n");
	}




	checkedClose(socket_desc);
}
