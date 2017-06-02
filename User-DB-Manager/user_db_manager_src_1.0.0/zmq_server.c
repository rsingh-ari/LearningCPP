/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

/** @brief ZMQ server program interfaces
 *  @file zmq_server.c
 *  @author Aricent
 *  @date Apr 2017
 *  @version 1.0
 *  
 *  This file creates a ZMQ server by using a predefined port number and
 *  receives the ZMQ client requests and parse them json string fromat.
 *  
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "zmq_server.h"
#include"db_management.h"

typedef int bool;
#define true 1
#define false 0
#define BUFFER_MAX_SIZE 25
#define FILE_PATH_MAX_SIZE 100


int counterTemp = 0;
extern char current_user[2][25];
extern char **user_list;

#define REPORT_NAME_LENGTH 25

//Defining key string constants
const char *COMMAND = "command";
const char *ARGUMENT = "argument";

const char *USER_NAME = "username";
const char *ROLE = "role";
const char *PASSWORD = "password";
const char *VALIDATION_TIME = "validationTime";
const char *VALIDATION_DATA = "data";
const char *VALIDATION_DIR_NAME = "validationDirName";

//Defining command string constants
const char *CREATE_REPORT = "createValidationReport";
const char *LOGIN_USER = "loginUser";
const char *LOGOUT_USER = "logoutUser";
const char *GET_CURRENT_USER = "getCurrentUser";
const char *ADD_USER = "addUser";
const char *DELETE_USER = "deleteUser";
const char *LIST_ALL_USERS = "listAllUsers";


bool CMD_CREATE_REPORT = false;
bool CMD_LOGIN_USER = false;
bool CMD_LOGOUT_USER = false;
bool CMD_GET_CURRENT_USER= false;
bool CMD_ADD_USER = false;
bool CMD_DELETE_USER = false;
bool CMD_LIST_ALL_USERS = false;


char user_name[BUFFER_MAX_SIZE], pswd[BUFFER_MAX_SIZE], user_role[BUFFER_MAX_SIZE], validation_time[BUFFER_MAX_SIZE], validation_data_path[BUFFER_MAX_SIZE], validation_dir_name[BUFFER_MAX_SIZE];
char report_name[FILE_PATH_MAX_SIZE], report_path[FILE_PATH_MAX_SIZE];
char *strValueUserList = NULL;


/** @brief Removes the first and last charectors of the string
 *
 *  This function removes the first and last double quotes from string.
 * 
 *  @param input The input string.
 *  @param output The output string.
 *  @return Void.
 */ 
void remove_first_and_last_char(const char *input, char *output)
{
  int len = strlen(input);
  if(len > 0)
    strcpy(output, ++input);
  if(len > 1)
    output[len - 2] = '\0';
}	

/** @brief Reset all the buffer
 *
 *  This function reset all the buffer values.
 * 
 *  @param Void.
 *  @return Void.
 */
void reset_all_buffer_values(void)
{
    memset(user_name, '\0', sizeof(user_name));
	memset(pswd, '\0', sizeof(pswd));
    memset(user_role, '\0', sizeof(user_role));
    memset(validation_time, '\0', sizeof(validation_time));
	memset(validation_data_path, '\0', sizeof(validation_data_path));
	memset(validation_dir_name, '\0', sizeof(validation_dir_name));
    memset(report_name, '\0', sizeof(report_name));
	memset(report_path, '\0', sizeof(report_path));
	
	CMD_CREATE_REPORT = false;
	CMD_LOGIN_USER = false;
	CMD_LOGOUT_USER = false;
	CMD_GET_CURRENT_USER= false;
	CMD_ADD_USER = false;
	CMD_DELETE_USER = false;
	CMD_LIST_ALL_USERS = false;	 
	if(strValueUserList != NULL)
	{
		free(strValueUserList);
		strValueUserList = NULL;
	}
}

/** @brief Make object of json array 
 *
 *  This function creates json object array and return the same.
 *  It retruns NULL if no user list is found.
 * 
 *  @param arraySize Sizse of array that need to be create as an object. 
 *  @return json_object This json object would be returned as array.
 */
json_object *make_array(int arraySize)
{
	json_object *my_array;
	int i;
	json_object *jstring[arraySize];

	my_array = json_object_new_array();

	/*Creating json strings*/
	for(i=0;i<arraySize;i++)
	{
		if(user_list[i] == NULL)
			return NULL;
		else			
		{
			jstring[i] = json_object_new_string(user_list[i]);
			json_object_array_add(my_array,jstring[i]);
		}
	}	
	
	return my_array;
}

/** @brief Print json values. 
 *
 *  This function prints the value corresponding to boolean, double, integer and strings. 
 * 
 *  @param jobj json object that need to print. 
 *  @return Void.
 */
void print_json_value(json_object *jobj)
{
  enum json_type type;
  //printf("type: ",type);
  type = json_object_get_type(jobj); /*Getting the type of the json object*/
  switch (type) {
    case json_type_boolean: printf("json_type_boolean\n");
                         printf("value: %s\n", json_object_get_boolean(jobj)? "true": "false");
                         break;
    case json_type_double: printf("json_type_double\n");
                        printf("          value: %lf\n", json_object_get_double(jobj));
                         break;
    case json_type_int: printf("json_type_intn");
                        printf("          value: %d\n", json_object_get_int(jobj));
                         break;
    case json_type_string: printf("json_type_string\n");
                         printf("          value: %s\n", json_object_get_string(jobj));
                         break;
  }

}

/** @brief parse json array. 
 *
 *  This function parse the json array values corresponding to boolean, double, integer and strings. 
 * 
 *  @param jobj json object that need to parse. 
 *  @param key Key value present in json object.
 *  @return Void.
 */
void json_parse_array( json_object *jobj, char *key)
{
  void json_parse(json_object * jobj); /*Forward Declaration*/
  enum json_type type;

  json_object *jarray = jobj; /*Simply get the array*/
  if(key) 
  {
   jarray = json_object_object_get(jobj, key); /*Getting the array if it is a key value pair*/
  }

  int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
#if DEBUG
  printf("Array Length: %d\n",arraylen);
#endif
  int i;
  json_object * jvalue;

  for (i=0; i< arraylen; i++)
  {
    jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
    type = json_object_get_type(jvalue);
    if (type == json_type_array)
	{
      json_parse_array(jvalue, NULL);
    }
    else if (type != json_type_object)
	{
#if DEBUG		
      printf("value[%d]: ",i);
#endif
      print_json_value(jvalue);
    }
    else
	{
      json_parse(jvalue);
    }
  }
}

/** @brief get and store json key and value. 
 *
 *  This function get and store the json key and values in global variables. 
 * 
 *  @param str String value present in json object.
 *  @param key Key value present in json object.
 *  @return Void.
 */
void get_json_key_value(const char *key, const char *str)
{
    char strValue[FILE_PATH_MAX_SIZE];
	remove_first_and_last_char(str, strValue);
	
	if( strcmp(key, COMMAND) == 0)
	{
		printf("\t%s : %s\n", key,strValue);
		
		if( strcmp(strValue, CREATE_REPORT) == 0)				
		{ 
	#if DEBUG		
			printf("Call Create report function here\n");
	#endif
			CMD_CREATE_REPORT = true;
		}
		else if( strcmp(strValue, LOGIN_USER) == 0 )
		{
	#if DEBUG		
			printf("Call LOGIN_USER function here\n");
	#endif
			CMD_LOGIN_USER= true;
		}
		else if( strcmp(strValue, LOGOUT_USER) ==0)
		{ 
	#if DEBUG		
			printf("Call LOGOUT_USER function here\n");
	#endif
			CMD_LOGOUT_USER=true; 
		}
		else if( strcmp(strValue, GET_CURRENT_USER) == 0)
		{ 
	#if DEBUG		
			printf("Call GET_CURRENT_USER function here\n"); 
	#endif
			CMD_GET_CURRENT_USER = true;
		}
		else if( strcmp(strValue, ADD_USER) == 0)
		{ 
	#if DEBUG		
			printf("Call ADD_USER function here\n"); 
	#endif
			CMD_ADD_USER=true;
		}
		else if( strcmp(strValue, DELETE_USER) == 0)
		{ 
	#if DEBUG		
			printf("Call DELETE_USER function here\n");	
	#endif
			CMD_DELETE_USER=true;
		}
		else if( strcmp(strValue, LIST_ALL_USERS) == 0)
		{ 
	#if DEBUG		
			printf("Call LIST_ALL_USERS function here\n"); 
	#endif
			CMD_LIST_ALL_USERS=true;
		}
					  
	}		
	else if( strcmp(key, ARGUMENT) == 0)
	{
		printf("\t%s : %s\n", key,strValue);
	}
	else if( strcmp(key, USER_NAME) == 0)
	{
		printf("\t%s : %s\n", key,strValue);		
		strcpy(user_name, strValue);		
	}		
	else if( strcmp(PASSWORD, key) == 0)
	{
		printf("\t%s : %s\n", key,strValue);
		strcpy(pswd, strValue);
	}
	else if( strcmp(key, ROLE) == 0)
	{
		printf("\t%s : %s\n", key,strValue);
		strcpy(user_role, strValue);
	}
	else if( strcmp(key, VALIDATION_TIME) == 0)
	{
		printf("\t%s : %s\n", key,strValue);
		strcpy(validation_time, strValue);
	}
	else if( strcmp(key, VALIDATION_DIR_NAME) == 0)
	{
		printf("\t%s : %s\n", key,strValue);
		strcpy(validation_dir_name, strValue);
	}	
  }
  
/** @brief Get login user string as json object. 
 *
 *  This function create and returns the json object of login user. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
static char* execute_login_user_command(void)
{
	static char strValue[256];	 
    memset(strValue,'\0',256);

 	//char *errMsg1 = db_logout_user(get_data_base());
	
	char *errMsg = db_login_user(get_data_base(),user_name,pswd);	
	 
	/*Creating a json object*/	
	if(errMsg == NULL)
	{
		json_object * jobj = json_object_new_object();
		int rc = json_object_object_add(jobj, (const char*)USER_NAME, jobj);
		if (rc != -1)
		{
			printf("ERROR: able to successfully add object to itself!\n");
			fflush(stdout);
		}		
		json_object_object_add(jobj, (const char*)USER_NAME, json_object_new_string(current_user[0]));
		json_object_object_add(jobj, (const char*)ROLE, json_object_new_string(current_user[1]));
		strcpy(strValue,json_object_to_json_string(jobj));			
#if DEBUG
		printf ("execute_login_user_command Object created:  %s\n",strValue);
#endif
		json_object_put(jobj);
		
		return strValue;
	}			
	else
	{
		return "{}";
	}
}

/** @brief Get logout user string as json object. 
 *
 *  This function returns error message in case of failure otherwise NULL in error msg. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
static char* execute_logout_user_command(void)
{
	 static char strValue[256];
	 memset(strValue,'\0',256);
	 char errorMsg[100];
	 char *errMsg = db_logout_user(get_data_base());
	 
	/*Creating a json object*/	
	if(errMsg == NULL)
		strcpy(errorMsg,"");
	else
		strcpy(errorMsg,errMsg);	
	
	json_object * jobj = json_object_new_object();
	int rc = json_object_object_add(jobj, "error", jobj);
	if (rc != -1)
	{
		printf("ERROR: able to successfully add object to itself!\n");
		fflush(stdout);
	}		
	json_object_object_add(jobj, "error", json_object_new_string(errorMsg));
	strcpy(strValue,json_object_to_json_string(jobj));			
#if DEBUG	
	printf ("execute_logout_user_command Object created:  %s\n",strValue);
#endif
	json_object_put(jobj);
	
	return strValue;
}

/** @brief Get current user string as json object. 
 *
 *  This function returns {} in case of failure otherwise user name and role. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
static char* execute_current_user_command(void)
{
	 static char strValue[256];
	 memset(strValue,'\0',256);

	char *errMsg = db_current_user(get_data_base());
	/*Creating a json object*/	
	if(	strcmp(errMsg, "success") == 0)
	{
		//printf("Current user= %s role=  %s \n",current_user[0], current_user[1]);
		json_object * jobj = json_object_new_object();
		int rc = json_object_object_add(jobj, (const char*)USER_NAME, jobj);
		if (rc != -1)
		{
			printf("ERROR: able to successfully add object to itself!\n");
			fflush(stdout);
		}		
		json_object_object_add(jobj, (const char*)USER_NAME, json_object_new_string(current_user[0]));
		json_object_object_add(jobj, (const char*)ROLE, json_object_new_string(current_user[1]));
		strcpy(strValue,json_object_to_json_string(jobj));			
#if DEBUG		
		printf ("execute_current_user_command Object created:  %s\n",strValue);
#endif
		json_object_put(jobj);
		
		return strValue;
	}			
	else
	{
		return "{}" ;
	}
}

/** @brief get added user string as json object. 
 *
 *  This function returns error msg in case of failure otherwise return NULL in error msg. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
static char* execute_add_user_command(void)
{
	 static char strValue[256];
	 memset(strValue,'\0',256);
	 char *errMsg = db_add_user(get_data_base(),user_name,pswd,user_role,"logout");	 
	 char errorMsg[100];
	/*Creating a json object*/	
	if(errMsg == NULL)
		strcpy(errorMsg,"");
	else
		strcpy(errorMsg,errMsg);
	
	json_object * jobj = json_object_new_object();
	int rc = json_object_object_add(jobj, "error", jobj);
	if (rc != -1)
	{
		printf("ERROR: able to successfully add object to itself!\n");
		fflush(stdout);
	}		
	json_object_object_add(jobj, "error", json_object_new_string(errorMsg));
	strcpy(strValue,json_object_to_json_string(jobj));			
#if DEBUG	
	printf ("execute_add_user_command Object created:  %s\n",strValue);
#endif
	json_object_put(jobj);
	
	return strValue;
}

/** @brief get deleted user string as json object. 
 *
 *  This function returns error msg in case of failure otherwise return NULL in error msg. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
static char* execute_delete_user_command(void){
	 static char strValue[256];
	 memset(strValue,'\0',256);
	char *errMsg1 = db_current_user(get_data_base());
	
	if(	(strcmp(errMsg1, "success") == 0) && (strcmp(current_user[0], user_name) == 0))
		return "{\"error\": \"Can not delete, user is currenlty logged in.\"}";	
	 
	char *errMsg = db_delete_user(get_data_base(),user_name,user_role);	 
	char errorMsg[100];
	/*Creating a json object*/	
	if(errMsg == NULL)
		strcpy(errorMsg,"");
	else
		strcpy(errorMsg,errMsg);
	
	json_object * jobj = json_object_new_object();
	int rc = json_object_object_add(jobj, "error", jobj);
	if (rc != -1)
	{
		printf("ERROR: able to successfully add object to itself!\n");
		fflush(stdout);
	}		
	json_object_object_add(jobj, "error", json_object_new_string(errorMsg));
	strcpy(strValue,json_object_to_json_string(jobj));			
#if DEBUG	
	printf ("execute_delete_user_command Object created:  %s\n",strValue);
#endif
	json_object_put(jobj);
	
	return strValue;
}

/** @brief Generate the validation report and upadte the database.
 *
 *  This function returns error msg in case of failure otherwise return report name. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
static char* execute_validation_report_command(void)
{
	static char strValue[256];
	memset(strValue,'\0',256);
	char reportPath[100];
	memset(reportPath,'\0',100);

	char *errMsg1 = db_current_user(get_data_base());
	//char *filePaths[4]={path_zero_air, path_2_ppm, path_10_ppm, path_100_ppm};
	if(	strcmp(errMsg1, "success") == 0)
	{
		if(validation_dir_name == NULL)
			return "{\"error\": \"Validation directory path not found. PDF report can not generate.\"}";
		printf("Inserting Calibration Data in DB............\n");
		strcpy(reportPath,get_ch4_ppm_update_db(validation_time,current_user[0],validation_dir_name));
		if(reportPath == NULL)
			return "{\"error\": \"PDF not generated\"}";
		else
		{
			/*Creating a json object*/	
			json_object * jobj = json_object_new_object();
			int rc = json_object_object_add(jobj, "reportname", jobj);
			if (rc != -1)
			{
				printf("ERROR: able to successfully add object to itself!\n");
				fflush(stdout);
			}		
			json_object_object_add(jobj, "reportname", json_object_new_string(reportPath));
			//json_object_object_add(jobj, "path", json_object_new_string(report_path));
			strcpy(strValue,json_object_to_json_string(jobj));			
#if DEBUG			
			printf ("execute_validation_report_command Object created:  %s\n",strValue);
#endif			
			json_object_put(jobj);
			
			return strValue;
		}
	}
	else
	{
		return "{\"error\": \"PDF not generated\"}" ;
	}
	
}

/** @brief get user list present in database string as json object. 
 *
 *  This function returns {} in case of no user present otherwise return user list. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
char* execute_list_users_command(void)
{
	int userCount = db_get_user_list(get_data_base());	 
	strValueUserList = (char *)malloc(sizeof(char)*userCount*(BUFFER_MAX_SIZE+6));
	if(userCount>0)
	{
		/*Creating a json object*/	
		json_object * jobj = json_object_new_object();
		int rc = json_object_object_add(jobj, "users", jobj);
		if (rc != -1)
		{
			printf("ERROR: able to successfully add object to itself!\n");
			fflush(stdout);
		}		
		
		/*Creating a json array*/
		json_object *jarray = make_array(userCount);//json_object_new_array();
		if(jarray == NULL)	
		{
			json_object_put(jobj);
			return "{\"error\": \"Error in returning user list\"}";
		}		
		else
		{
			json_object_object_add(jobj,"users", jarray);
			strcpy(strValueUserList,json_object_to_json_string(jobj));			
#if DEBUG			
			printf ("execute_list_users_command Object created:  %s\n",strValueUserList);
#endif
			json_object_put(jobj);	
			reset_user_list();
			return strValueUserList;
		}
	}
	else
	{
		return "{}" ;
	}
	
}

/** @brief Validate and execute the commands received in json request. 
 *
 *  This function returns error msg in case of failure otherwise call the
 *  the appropiate function. 
 * 
 *  @param Void. 
 *  @return char* Returns string in json format.
 */
char* validate_and_execute_commands(void)
{	
	//printf("Calling validate_and_execute_commands functions :\n");

	if( CMD_CREATE_REPORT )
	{
#if DEBUG	
		printf("Call %s function.\n", CREATE_REPORT);
#endif
		//Check weather command has complete message or not
		if(validation_time == NULL || validation_dir_name == NULL)
			return "{\"error\": \"Command parameters are incomplete!\"}";
		else
			return execute_validation_report_command();
		
	}
	else if( CMD_LOGIN_USER )
	{
#if DEBUG	
		printf("Call %s function. with username= %s, password = %s \n", LOGIN_USER, user_name, pswd);
#endif
		//Check weather command has complete message or not
		if(user_name == NULL || pswd == NULL)
			return "{\"error\": \"Command parameters are incomplete!\"}";
		else
			return execute_login_user_command();
		
	}	
	else if( CMD_LOGOUT_USER)
	{
#if DEBUG	
		printf("Call %s function.\n", LOGOUT_USER);
#endif
		return execute_logout_user_command();
		
	}	
	else if( CMD_GET_CURRENT_USER)
	{
#if DEBUG	
		printf("Call %s function.\n", GET_CURRENT_USER);
#endif
		return execute_current_user_command();
		
	}
	else if( CMD_ADD_USER)
	{
#if DEBUG	
		printf("Call %s function.\n", ADD_USER);
#endif
		if(user_name == NULL || pswd == NULL || user_role== NULL)
			return "{\"error\": \"Command parameters are incomplete!\"}";
		else
			return execute_add_user_command();		
		
	}
	else if( CMD_DELETE_USER)
	{
#if DEBUG	
		printf("Call %s function.\n", DELETE_USER);
#endif
		if(user_name == NULL || user_role== NULL)
			return "{\"error\": \"Command parameters are incomplete!\"}";
		else
			return execute_delete_user_command();
		
	}
	else if( CMD_LIST_ALL_USERS)
	{
#if DEBUG	
		printf("Call %s function.\n", LIST_ALL_USERS);
#endif
		return execute_list_users_command();
	}
	else
	{
		return "{\"error\": \"Invalid JSON format or no command found!\"}";
	}	
	
}
  

/** @brief parse the json object. 
 *
 *  This function returns error msg in case of failure otherwise call the
 *  the appropiate functions. 
 * 
 *  @param jobj Json object received from client program. 
 *  @return Void.
 */
void json_parse(json_object * jobj) 
{
	enum json_type type;
	counterTemp = counterTemp+1;
	
	json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/  

		type = json_object_get_type(val);
		printf("counterTemp:%d \n",counterTemp);
		//printf("type: \n",type);
	    switch (type) {
	      case json_type_boolean: 
	      case json_type_double: 
	      case json_type_int: 		  						
	      case json_type_string: 
		  	{
					printf("json_parse: key = %s\n", key);
					print_json_value(val);					
	                           break; 
	      	}							   
	      case json_type_object: printf("json_type_object\n");
	                           jobj = json_object_object_get(jobj, key);
	                           json_parse(jobj); 
	                           break;
	      case json_type_array: printf("type: json_type_array, ");
	                          json_parse_array(jobj, key);
	                          break;
	      case json_type_null: printf("value is null \n");
		  					  break;
	    }
	 }

} 

/** @brief sort function. 
 *
 *  This function sorts the json object integer values. 
 *  
 *  @return int Returns the difference of integers.
 */
static int sort_fn (const void *j1, const void *j2)
{
	json_object * const *jso1, * const *jso2;
	int i1, i2;

	jso1 = (json_object* const*)j1;
	jso2 = (json_object* const*)j2;
	if (!*jso1 && !*jso2)
		return 0;
	if (!*jso1)
		return -1;
	if (!*jso2)
		return 1;

	i1 = json_object_get_int(*jso1);
	i2 = json_object_get_int(*jso2);

	return i1 - i2;
}

/** @brief Thread function to create the ZMQ server. 
 *
 *  This function creates the ZMQ server and receives the json request from the ZMQ client program.
 * 
 *  @param arg require for program.
 *  @return Void* Returns the function values as per the need.
 */
void* zmq_server_thread(void *arg)
{
	//	Socket to talk to clients
	void *context = zmq_ctx_new ();
	void *responder = zmq_socket (context, ZMQ_REP);
	int rc =0;
	rc = zmq_bind (responder, "tcp://*:5559");
	char buffer [500];	
	//assert (rc == 0);
	while (1) {
		//char buffer [10];
	        printf ("\n\n************** Waiting for ZMQ Request *******************\n");
		reset_all_buffer_values();	
		zmq_recv (responder, buffer, 500, 0);
		printf ("\nJSON Request :\n");

		json_object * jobj = json_tokener_parse(buffer);
		if(jobj != NULL)
		{		
		  json_object_object_foreach(jobj, key, val)
		  {
			const char *strValue = json_object_to_json_string(val);
			//printf("\t%s: %s\n", key, strValue);
			if(strValue != NULL)	
			  get_json_key_value(key, strValue);	
		  }
		}
		char *tempBuff = validate_and_execute_commands();					
		printf ("\nJSON Response :\n\t%s \n",tempBuff);
		zmq_send (responder, tempBuff, strlen(tempBuff), 0);		
		reset_all_buffer_values();
		json_object_put(jobj);
		
	}
	return EXIT_SUCCESS;
}
