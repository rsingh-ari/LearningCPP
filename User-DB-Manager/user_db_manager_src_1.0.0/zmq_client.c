//  Hello World client
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <zmq.h>
#include <json-c/json.h>

#include "file_parser.h"
#include "db_management.h"


//#include <conio.h>

//static const char *JSON_STRING =	"{\"user\": \"johndoe\"}";
static const char *ADD_USER[4] = {"{\"command\": \"addUser\", \"username\": \"user1\", \"role\": \"admin\", \"password\": \"password\"}", 
								  "{\"command\": \"addUser\", \"username\": \"user2\", \"role\": \"operator\", \"password\": \"password\"}",
			   			          "{\"command\": \"addUser\", \"username\": \"user3\", \"role\": \"operator\", \"password\": \"password\"}",
						          "{\"command\": \"addUser\", \"username\": \"user4\", \"role\": \"operator\", \"password\": \"password\"}" };

static const char *DELETE_USER[4] =	{ "{\"command\": \"deleteUser\", \"username\": \"user1\", \"role\": \"admin\"}",
									  "{\"command\": \"deleteUser\", \"username\": \"user2\", \"role\": \"operator\"}",
  									  "{\"command\": \"deleteUser\", \"username\": \"user3\", \"role\": \"operator\"}",
									  "{\"command\": \"deleteUser\", \"username\": \"user4\", \"role\": \"operator\"}" };

static const char *LOGIN_USER[4] = { "{\"command\": \"loginUser\", \"username\": \"user1\", \"password\": \"password\"}",
									 "{\"command\": \"loginUser\", \"username\": \"user2\", \"password\": \"password\"}",
 									 "{\"command\": \"loginUser\", \"username\": \"user3\", \"password\": \"password\"}",
									 "{\"command\": \"loginUser\", \"username\": \"user4\", \"password\": \"password\"}" };

static const char *LIST_USERS =	"{\"command\": \"listAllUsers\"}";
static const char *LOG_OUT_USER =	"{\"command\": \"logoutUser\"}";
static const char *GET_GURRENT_USER =	"{\"command\": \"getCurrentUser\"}";

static const char *EXIT_SERVER =	"{\"command\": \"exitServer\"}";


static const char *CREATE_V_REPORT = "{\"command\": \"createValidationReport\", \"validationTime\": \"170525 02:58\",\
										\"validationDirName\": \"20170222\"}";

int main (void)
{

    printf ("Connecting to User and DB manager server...\n");
    char buffer [500];
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5559");
    //memset(buffer,'\0',500);
    int request_nbr;
	int numberValue =0;
	int userNumber=0;
    //for (request_nbr = 0; request_nbr != 1; request_nbr++) {
    while (1){
		printf ("Enter command number to execute:\n\tEnter 0 to create validation report. \n\tEnter 1 for add user. \
			\n\tEnter 2 for delete.\n\tEnter 3 for login.\n\tEnter 4 for logout. \
			\n\tEnter 5 for get current user.\n\tEnter 6 for list all users.\n\tEnter 7 to exit\n");
		scanf ("%d", &numberValue);
		memset(buffer,'\0',500);
		switch(numberValue)
		{
			case 0: 
				printf ("Sending CREATE_REPORT command \n");				
				zmq_send (requester, CREATE_V_REPORT,strlen(CREATE_V_REPORT), 0);
				break;
			case 1:
				printf ("Enter user number to add as:\n 1 for user1\n 2 for user2\n 3 for user3 \n 4 for user4\n");				
				scanf ("%d", &userNumber);
				if(userNumber < 1 || userNumber > 4)
					userNumber = 1;
				printf ("Sending ADD_USER command \n");				
				zmq_send (requester, ADD_USER[userNumber-1],strlen(ADD_USER[userNumber-1]), 0);
				break;
			case 2: 
				printf ("Enter user number to delete as:\n 1 for user1\n 2 for user2\n 3 for user3 \n 4 for user4\n");				
				scanf ("%d", &userNumber);
				if(userNumber < 1 || userNumber > 4)
					userNumber = 1;
				printf ("Sending DELETE_USER command \n");				
				zmq_send (requester, DELETE_USER[userNumber-1],strlen(DELETE_USER[userNumber-1]), 0);			
				break;
			case 3: 
				printf ("Enter user number to login as:\n 1 for user1\n 2 for user2\n 3 for user3 \n 4 for user4\n");
				scanf ("%d", &userNumber);
				if(userNumber < 1 || userNumber > 4)
					userNumber = 1;
				printf ("Sending LOGIN_USER command \n");				
				zmq_send (requester, LOGIN_USER[userNumber-1],strlen(LOGIN_USER[userNumber-1]), 0);

				break;
			case 4: 
				printf ("Sending LOG_OUT_USER command \n");				
				zmq_send (requester, LOG_OUT_USER,strlen(LOG_OUT_USER), 0);
				break;	
			case 5: 
				printf ("Sending GET_GURRENT_USER command \n");				
				zmq_send (requester, GET_GURRENT_USER,strlen(GET_GURRENT_USER), 0);
				break;
			case 6: 
				printf ("Sending LIST_USERS command \n");
				zmq_send (requester, LIST_USERS,strlen(LIST_USERS), 0);
				break;				
			case 7:
				//Need to comment this as this is for testing purpose
				//zmq_send (requester, EXIT_SERVER,strlen(EXIT_SERVER), 0);
				exit(0);
			default: 
				printf ("Sending LIST_USERS command \n");
				zmq_send (requester, LIST_USERS,strlen(LIST_USERS), 0);
				break;	
				
		};
        zmq_recv (requester, buffer, 500, 0);
        printf ("Received ZMQ reply as : %s \n", buffer);
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
  //  getch();
    return 0;
}
