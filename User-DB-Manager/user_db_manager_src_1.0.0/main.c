/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

/** @brief main User and DB Manager program
 *  @file main.c
 *  @author Aricent
 *  @date Apr 2017
 *  @version 1.0
 *  
 *  This file create the database tables and create ZMQ server thread program.
 *  
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
//#include <sqlite3.h>

#include "db_management.h"
#include "file_parser.h"
#include "pdf_generator.h"
#include "debug.h"
#include "zmq_server.h"

#define MAX_GAS_SOURCE 4
sqlite3 *db;
void thread_create();
pthread_t t1;

/** @brief Get database
 *
 *  This function returns the open database descriptor.
 *  
 *  @return Void*.
 */
sqlite3 *get_data_base()
{
	return db;
}

/** @brief Create Thread.
 *
 *  This function create thread for ZMQ server program.
 * 
 *  @return Void.
 */
void thread_create()
{
	int rc;
	rc=pthread_create(&t1,NULL,zmq_server_thread,NULL);
	if (rc != 0) 
	{
	printf("Creation of ZMQ Server thread failed.\n");
	exit(-1);
	}
	else
	{
#if DEBUG
	printf("ZMQ Server thread created successfully.\n");
#endif
	}
}

/** @brief Entry point function for User and DB Manager program.
 *
 *  This function create the database tables and create ZMQ server thread program.
 * 
 *  @return int return 0 on success.
 */
int main()
{
      int rc;
      putenv("LD_LIBRARY_PATH=/usr/local/lib/");	
      /* Open database */
      rc = sqlite3_open("/usr/local/bin/user_db_config/user_db_manager.db", &db);
      if( rc )
        {
#if DEBUG
        print("Can't open database: %s\n", sqlite3_errmsg(db));
#endif
        return(0);
        }
	else
        {
#if DEBUG
        print("Opened database successfully\n");
#endif
        }

	/* Now creates database tables Gas table and login table if they do not exist in database*/
	create_database_tables(db);
	
	/*launch ZMQ Server thread*/
	thread_create();
	
	/*wait for thread to joinback after exit.*/
	pthread_join(t1,NULL);
	
	/*close the database*/
    sqlite3_close(db);
	return 0;
}