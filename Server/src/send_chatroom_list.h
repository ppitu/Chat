#include "lib.h"
#include "avltree.h"

#include <json-c/json.h>

void SendChatRoomList(int connect_desc, NodeAvlServer *root_avl_tree)
{
    int			*id_arr;
    char 		**server_name;
    size_t      avl_tree_size;

    //Creating a json object
	struct json_object *jobj = json_object_new_object();

	//Creating a json array of id
	struct json_object *jidarray = json_object_new_array();

	//Creatint a json array of serwer name
	struct json_object *jserwer_namearray = json_object_new_array();


    id_arr = AvlTreeReturnIdArray(root_avl_tree);

	server_name = AvlTreeReturnServerNameArray(root_avl_tree);

	avl_tree_size = AvlTreeSize(root_avl_tree);

    for(int i = 0; i < avl_tree_size; i++)
	{
		//creating a json integer
		json_object *jint = json_object_new_int(*(id_arr + i));

		//creating a json string

		json_object *jstring = json_object_new_string(*(server_name + i));

		//add json int to the array
		json_object_array_add(jidarray, jint);
		//add json string to the array
		json_object_array_add(jserwer_namearray, jstring);
	}


	//form the json object

	json_object_object_add(jobj, "Array", jidarray);
	json_object_object_add(jobj, "Server Name", jserwer_namearray);

    printf("The josn %s\n", json_object_to_json_string(jobj));

	checkedWrite(connect_desc, json_object_to_json_string(jobj), strlen(json_object_to_json_string(jobj)));
}