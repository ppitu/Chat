#include "lib.h"
#include "avltree.h"

int CreateJoinServerUserChoice(NodeAvlServer **root_avl_tree, int connfd, int socket_desc)
{
    int     choice, id;
    int     pom = 0;
    char    server_name[NAMELENGTH];
	char 	json_buffer[1024];

	struct json_object *paresd_json;
	struct json_object *jserver_name;

    checkedRead(connfd, &choice, sizeof(choice));

	if(choice == 1)
	{
		while(pom == 0)
		{
			checkedRead(connfd, &id, sizeof(id));
		
			if(AvlTreeContainId(*root_avl_tree, id))
			{
				pom++;
			}
			
			checkedWrite(connfd, &pom, sizeof(pom));
		}

	}else 
	{
        id = AvlTreeFindSmallestMissingIdValue(*root_avl_tree);
		while(pom == 0)
		{
			checkedRead(connfd, json_buffer, sizeof(json_buffer));

			paresd_json = json_tokener_parse(json_buffer);
			json_object_object_get_ex(paresd_json, "Server Name", &jserver_name);


			strcpy(server_name,  json_object_get_string(jserver_name));
			
			if(!AvlTreeContainServerName(*root_avl_tree, server_name ,id))
			{
				pom++;
				printf("Create new chatroom id: %d and name: %s\n", id, server_name);
				*root_avl_tree = AvlTreeInsert(*root_avl_tree, id, socket_desc, server_name);
			}
			
			checkedWrite(connfd, &pom, sizeof(pom));
		}
	}

    return id;
}