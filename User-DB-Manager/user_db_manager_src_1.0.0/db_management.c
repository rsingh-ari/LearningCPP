/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

/** @brief SQL insert program
 *  @file db_management.c
 *  @author Aricent
 *  @date Apr 2017
 *  @version 1.0
 *  
 *  This file used for the database management for User and DB management tool.
 *  
 */ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>

#include"db_management.h"
#include"debug.h"

#define TRUE 1
#define FALSE 0
#define MAX_GAS_SOURCE 4

char current_user[2][25];
char **user_list;
int user_count;
int user_index;
int  current_user_result = FALSE;
char *zErrMsg;

/** @brief Reset user list.
 *
 *  This function free the allocated memory of the user list.
 * 
 *  @return Void.
 */
void reset_user_list()
{
	int i;
	//printf("reset_user_list    user_index=%d\tuser_list=%p\n",user_index,user_list);
	if(user_list!=NULL)
	{
	for(i=0;i<user_index;i++)
		{
		//printf("reset_user_list     user_list[%d]= %p\n",i,user_list[i]);
		if(user_list[i] !=NULL)
	  	     free(user_list[i]);
	  	     user_list[i] = NULL;	
		}
	free(user_list);
	user_list = NULL;
	}
	user_index = 0;
}

/** @brief Callback function for SQL query
 *
 *  This function is register with SQLite database.
 * 
 *  @param NotUsed passd value to callback function.
 *  @param argc Total number of data counts.
 *  @param argv Total values of data counts.
 *  @param azColName Database table column name.
 *  @return int return 0 on success.
 */
static int create_callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


/** @brief Callback function for SQL query
 *
 *  This function is register with SQLite database.
 * 
 *  @param NotUsed passd value to callback function.
 *  @param argc Total number of data counts.
 *  @param argv Total values of data counts.
 *  @param azColName Database table column name.
 *  @return int return 0 fos success.
 */
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
#if DEBUG
   printf("NotUsed=%s\targc=%d\n",(char*)NotUsed,argc);
#endif
   if(strcmp((const char *)NotUsed,"current_user")==0)
   { 
   for(i=0; i<argc; i++){
#if DEBUG
      print("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
#endif
      if(strcmp(azColName[i],"USER_NAME")==0)
	{
	memset(current_user[0],'\0',25);
	strcpy(current_user[0],argv[i]);
	}
      if(strcmp(azColName[i],"USER_ROLE")==0)
	{
	memset(current_user[1],'\0',25);
	strcpy(current_user[1],argv[i]);
	}
	current_user_result = TRUE;
   }
#if DEBUG
   print("\n");
#endif
   }
   else if(strcmp((const char *)NotUsed,"user_list")==0)
      {
        for(i=0; i<argc; i++){
#if DEBUG
          print("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
#endif
	      if(strcmp(azColName[i],"USER_NAME")==0)
              {
              user_list[user_index]=(char*)malloc((strlen(argv[i])+1));  
	      if(user_list[user_index] == NULL)
		{
		zErrMsg = "user list memory allocation failed\n";	
		}
	      else
		{
                strcpy(user_list[user_index],argv[i]);
                user_index++;
		}
              }
	}
#if DEBUG
   print("\n");
#endif
     }	
   else if(strcmp((const char *)NotUsed,"user_count")==0)
      {
	i=0;
	user_count = atoi(argv[i]);
#if DEBUG
          print("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
#endif
     }
   return 0;
}

/** @brief Insert gas table.
 *
 *  This function is used to create the surrogate gas table.
 * 
 *  @param db1 Open descriptor of database.
 *  @param date_time Current date and time.
 *  @param user_name Validator name.
 *  @param file_path Path of calibration data files.
 *  @param gas_src Gas source (zero air, ch42ppm,ch410ppm, ch4100ppm).
 *  @param Nmh4ppm Nominal methene ppm.
 *  @param avg Average of calibration data.
 *  @param SD Standard daviasion.
 *  @return int return 0 on success.
 */
int insert_gas_table(sqlite3 *db,char *date_time,char *user_name,char *file_path,char *gas_src,float Nmh4ppm,float avg,float SD)
{
   int i;   
   //char *zErrMsg = 0;
   int rc;
   char sql[250];
   memset(sql,'\0',250);

#if DEBUG
   //print("acg === %f\n",avg);
#endif
   sprintf(sql,"INSERT INTO GAS_TABLE VALUES('%s','%s','%s','%s',%f,%f,%f);",date_time,user_name,file_path,gas_src,Nmh4ppm,avg,SD);
#if DEBUG
   //print("%s\n",sql);
#endif
  /* Create SQL statement */
     //if(db!=NULL)
       { 
   	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   	if( rc != SQLITE_OK ){
#if DEBUG
      	print("SQL error: %s\n", zErrMsg);
#endif
      	sqlite3_free(zErrMsg);
   	}else{
#if DEBUG
      	print("GAS_TABLE Records created successfully\n");
#endif
   	}
       }
   return 0;
}

/** @brief Insert calibration table.
 *
 *  This function is used to create the calibration table.
 * 
 *  @param db1 Open descriptor of database.
 *  @param date_time Current date and time.
 *  @param user_name Validator name.
 *  @param val Calibration values.
 *  @return int return 0 on success.
 */
int insert_to_calibration_table(sqlite3 *db,char *date_time,char *user_name,float val[])
{
	char sql[250];	
	//char *zErrMsg = 0;
	int rc;
	memset(sql,'\0',250);
	//printf("%f\t%f\t%f\t%f\t",val[0],val[1],val[2],val[3]);
	sprintf(sql,"INSERT INTO CALIBRATION_TABLE VALUES('%s','%s','%f','%f','%f',%f);",date_time,user_name,val[0],val[1],val[2],val[3]);
   	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   	if( rc != SQLITE_OK ){
#if DEBUG
      	print("SQL error: %s\n", zErrMsg);
#endif
      	sqlite3_free(zErrMsg);
   	}else{
#if DEBUG
      	print("CALIBRATION_TABLE Records created successfully\n");
#endif
   	}
}

/** @brief Add user in database.
 *
 *  This function add user details in database table.
 * 
 *  @param db1 Open descriptor of database.
 *  @param name User name.
 *  @param pd Password.
 *  @param user_role User role.
 *  @param user_status User status.
 *  @return char return NULL on success.
 */
char* db_add_user(sqlite3 *db,char *name,char *pd,char *user_role,char *user_status)
{
	char sql[150];	
   	//char *zErrMsg = 0;
   	int rc;
	memset(sql,'\0',150);
#if DEBUG
	print("name=%s\tpd=%s\tuser_role=%s\tuser_status=%s\n",name,pd,user_role,user_status);
#endif
	sprintf(sql,"INSERT INTO USER_ACCOUNT (USER_NAME,PASSWORD,USER_ROLE,USER_STATUS) VALUES('%s','%s','%s','%s');",name,pd,user_role,user_status);
   	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   	if( rc != SQLITE_OK ){
#if DEBUG
      	print("SQL error: %s\n", zErrMsg);
#endif
      	sqlite3_free(zErrMsg);
	return zErrMsg;
   	}else{
#if DEBUG
      	print("USER_ACCOUNT Records created successfully\n");
#endif
   	}
	return "NULL";
}

/** @brief Delete user in database.
 *
 *  This function delete user details from database table.
 * 
 *  @param db1 Open descriptor of database.
 *  @param name User name.
  *  @param user_role User role. 
 *  @return char return NULL on success.
 */
char *db_delete_user(sqlite3 *db,char *user_name,char *user_role)
{
   char sql[100];   
   //char *zErrMsg = 0;
   int rc;
   const char* data = "Callback function called";
   memset(sql,'\0',100);

   sprintf(sql,"DELETE from USER_ACCOUNT where USER_NAME='%s' AND USER_STATUS!='login';",user_name);
   //sprintf(sql,"UPDATE USER_ACCOUNT SET USER_STATUS='deleted' where USER_NAME='%s';",user_name);
   //sprintf(sql,"UPDATE USER_ACCOUNT SET USER_STATUS='login' where USER_NAME='%s' AND PASSWORD='%s';",user_name,passwd);
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      return zErrMsg;
   }else{
#if DEBUG
      fprintf(stdout, "delete done successfully\n");
#endif
   }
   //sqlite3_close(db);
   return NULL;
}

/** @brief Login user in database.
 *
 *  This function login user in database table.
 * 
 *  @param db1 Open descriptor of database.
 *  @param name User name.
 *  @param pd Password.
 *  @return char return user name and role on success.
 */
char *db_login_user(sqlite3 *db,char *user_name,char *passwd)
{
   char sql[150];
   char *funcValue;   
   //char *zErrMsg = 0;
   int rc;
   const char* data = "Callback function called";
   memset(sql,'\0',150);
   funcValue = db_logout_user(db);
   sleep(2); //TODO : remove sleep.
   if(funcValue == NULL)
   {
     sprintf(sql,"UPDATE USER_ACCOUNT SET USER_STATUS='login' where USER_NAME='%s' AND PASSWORD='%s';",user_name,passwd);
     rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
     if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return zErrMsg;
     }else{
#if DEBUG
        printf("status updated successfully\n");
#endif
   	funcValue = db_current_user(db);
     }
     return NULL;
   }
   return funcValue;    
}

/** @brief logout user in database.
 *
 *  This function logout user in database table.
 * 
 *  @param db1 Open descriptor of database. 
 *  @return char return NULL on success.
 */
char *db_logout_user(sqlite3 *db)
{
   char sql[100];   
   //char *zErrMsg = 0;
   int rc;
   const char* data = "Callback function called";
   memset(sql,'\0',100);
   sprintf(sql,"UPDATE USER_ACCOUNT SET USER_STATUS='logout' where USER_STATUS='login';");
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      return zErrMsg;
   }else{
#if DEBUG
      printf("user logout status updated successfully\n");
#endif
   }

   return NULL;
}

/** @brief Get current login user in database.
 *
 *  This function gets current login user in database table.
 * 
 *  @param db1 Open descriptor of database. 
 *  @return char return user name and role on success.
 */
char *db_current_user(sqlite3 *db)
{
   char sql[100];   
   //char *zErrMsg = 0;
   int rc;
   const char* data = "current_user";
   memset(sql,'\0',100);
   sprintf(sql,"SELECT * from USER_ACCOUNT where USER_STATUS='login';");
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      return zErrMsg;
   }else{
	if(current_user_result)
	{
#if DEBUG
            printf("current login status success\n");
#endif
      	    current_user_result=FALSE;
	    zErrMsg = "success";
	    return zErrMsg;
	}
	else
	{
#if DEBUG
		 printf("current login status failuer\n");
#endif
		return "NULL";
	}
   }
   return "NULL";
}

/** @brief Get user list in database table.
 *
 *  This function gets user list in database table.
 * 
 *  @param db1 Open descriptor of database. 
 *  @return char return user name on success.
 */
int db_get_user_list(sqlite3 *db)
{
   char sql[100];   
   //char *zErrMsg = 0;
   int rc,count=0;
   const char* data = "user_list";
   memset(sql,'\0',100);
        
   count = db_update_user_count(db);
   if(count > 0)
   {
   user_list=(char**)malloc(count*sizeof(char*));
   if(user_list == NULL)
	{
	return -1;
	}
   else
	{
   //printf("db_get_user_list    user_list=%p\n",user_list);
   sprintf(sql,"SELECT * from USER_ACCOUNT");
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return -1;
   	}
	else{
#if DEBUG
            printf("user list received success\n");
#endif
		}
   	}
   }
   return count;
}

/** @brief update user count.
 *
 *  This function update user counts in global variables..
 * 
 *  @param db1 Open descriptor of database. 
 *  @return int return user count.
 */
int db_update_user_count(sqlite3 *db)
{
   char sql[100];   
   //char *zErrMsg = 0;
   int rc,count=0;
   const char* data = "user_count";
   memset(sql,'\0',100);
   sprintf(sql,"SELECT count(USER_NAME) from USER_ACCOUNT");
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
      return -1;
   }else{
#if DEBUG
            printf("user count success\n");
#endif
	if(user_count > 0)
        {
                count = user_count;
                user_count = 0;
        }
   }
   return count;
}

/** @brief Create database tables
 *
 *  This function creates database tables.
 * 
 *  @param db Open descripter of database.
 *  @return Void.
 */
void create_database_tables(sqlite3 *db)
{
	//Creating gas table
	create_gas_table(db);
	
	//Creating user login table
	creat_login_table(db);
}

/** @brief Create gas tables
 *
 *  This function creates gas database tables.
 * 
 *  @param db Open descripter of database.
 *  @return Void.
 */
void create_gas_table( sqlite3 *db)
{
	char *sql;
        char *zErrMsg = 0;
	int rc;
        /* Create SQL statement */
 //       sql = "DROP TABLE IF EXISTS GAS_TABLE;"
         sql="CREATE TABLE IF NOT EXISTS GAS_TABLE("  \
	 "DATE_TIME CHAR(50) NOT NULL ,"\
	 "USER_NAME CHAR(50) NOT NULL ,"\
	 "FILE_PATH CHAR(50) NOT NULL ,"\
         "GAS_SOURCE CHAR CHAR(15)," \
         "Nominal_Methane_PPM REAL NOT NULL," \
         "Mean_Observed_Concentration REAL NOT NULL," \
         "SD REAL NOT NULL);"; \
	
       /* Exentration SQL statement */
       	rc = sqlite3_exec(db, sql, create_callback, 0, &zErrMsg);
       	if( rc != SQLITE_OK )
        {
#if DEBUG
        print("SQL error: %s\n", zErrMsg);
#endif
        sqlite3_free(zErrMsg);
        }
        else
        {
#if DEBUG
        print("GAS_TABLE Table created successfully\n");
#endif
        }
}

/** @brief Create login tables
 *
 *  This function creates login database tables.
 * 
 *  @param db Open descripter of database.
 *  @return Void.
 */
void creat_login_table(sqlite3 *db)
{
	
	char *sql;
        char *zErrMsg = 0;
	int rc;
        /* Create SQL statement */   
    	//sql ="DROP TABLE IF EXISTS USER_CREDINTIALS;"
	sql = "CREATE TABLE IF NOT EXISTS USER_ACCOUNT("  \
        "USER_NAME CHAR(50) UNIQUE NOT NULL,"\
	"PASSWORD CHAR(50) NOT NULL,"\
	"USER_ROLE CHAR(50) NOT NULL,"\
        "USER_STATUS CHAR(50) );"; \
	
       /* Exentration SQL statement */
       	rc = sqlite3_exec(db, sql, create_callback, 0, &zErrMsg);
       	if( rc != SQLITE_OK )
        {
#if DEBUG
        print("SQL ashok error: %s\n", zErrMsg);
#endif
        sqlite3_free(zErrMsg);
        }
        else
        {
#if DEBUG
        print("USER_CREDINTIALS Table created successfully\n");
#endif
        }
}
