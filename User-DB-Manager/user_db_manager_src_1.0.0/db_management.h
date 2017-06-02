/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

#include <sqlite3.h>

void create_database_tables(sqlite3 *);
void create_gas_table( sqlite3 *);
void creat_login_table(sqlite3 *);

int insert_gas_table(sqlite3 *,char *,char *,char *,char *,float ,float ,float );

char* db_add_user(sqlite3 *,char *,char *,char *,char *);

char* db_delete_user(sqlite3 *,char *,char *);
char *db_login_user(sqlite3 *,char *,char *);
char *db_logout_user(sqlite3 *);
void *get_dtabase();
char *get_current_user_name();
char *get_current_user_role();
char *db_current_user(sqlite3 *);
int db_get_user_list(sqlite3 *);
int db_update_user_count(sqlite3 *);
void reset_user_list();
sqlite3 *get_data_base();


