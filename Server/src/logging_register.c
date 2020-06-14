#include "lib.h"

#include   <json-c/json.h>
#include    <mysql/mysql.h>

const char *loggingRegisterUser(int connfd, MYSQL *con)
{
    struct 	json_object *paresd_json;
	char json_buffer[1024];
	struct 	json_object *jdata;
	struct 	json_object *jnickname;
	struct 	json_object *jpassword;
	struct 	json_object *jobj;
	struct 	json_object *jint;
	bool 		exists;
    char query[254] = {0};
    char nickname[128];
	int 	confirm = 0;
	int 	send_int = 0;


    	//recv json log
	do 
	{
		checkedRead(connfd, json_buffer, sizeof(json_buffer));
		confirm = 0;

		paresd_json = json_tokener_parse(json_buffer);

		exists = json_object_object_get_ex(paresd_json, "Logging", &jdata);

		if(exists == false)
		{
			exists = json_object_object_get_ex(paresd_json, "Register", &jdata);

			if(exists == false)
			{
				exists = json_object_object_get_ex(paresd_json, "Guest", &jdata);
				jnickname = json_object_array_get_idx(jdata, 0);
			}
			else 
			{
				jnickname = json_object_array_get_idx(jdata, 0);
				jpassword = json_object_array_get_idx(jdata, 1);


				snprintf(query, 254, "INSERT INTO users(username, password) VALUES ('%s', '%s')", json_object_get_string(jnickname), json_object_get_string(jpassword));

				if(mysql_query(con, query))
				{
					printf("Error %u: %s\n", mysql_errno(con), mysql_error(con));
					if(mysql_errno(con) == 1062)
					{
						confirm = 1;
					}
					else 
						exit(1);
				}
				else {
					confirm = 0;
				}

			}
		} 
		else 
		{
			jnickname = json_object_array_get_idx(jdata, 0);
			jpassword = json_object_array_get_idx(jdata, 1);

			snprintf(query, 254, "SELECT * FROM users WHERE username = '%s' AND password = '%s'", json_object_get_string(jnickname), json_object_get_string(jpassword));

			if(mysql_query(con, query))
			{
				printf("Error %u: %s\n", mysql_errno(con), mysql_error(con));
				exit(1);
			}

			MYSQL_RES *result = mysql_store_result(con);

			int num_fields = mysql_num_fields(result);

			MYSQL_ROW row = mysql_fetch_row(result);

			do
			{ 
				if(row == NULL)
				{
					confirm = 1;
					break;
				}	
    			for(int i = 1; i < num_fields; i++) 
    			{ 
        			printf("%s ", row[i] ? row[i] : "NULL"); 
    			} 
        		printf("\n"); 
			}
			while ((row = mysql_fetch_row(result)));

			mysql_free_result(result);
		}

		printf("%d\n", confirm);

		jobj = json_object_new_object();
		jint = json_object_new_int(confirm);
		json_object_object_add(jobj, "Confirm", jint);

		checkedWrite(connfd, json_object_to_json_string(jobj), strlen(json_object_to_json_string(jobj)));
		
	}while(confirm != 0);

	printf("%s\n", json_object_get_string(jnickname));
	printf("%s\n", json_object_get_string(jpassword));

    strcpy(nickname, json_object_get_string(jnickname));

    //return(nickname);
    return(json_object_get_string(jnickname));
}