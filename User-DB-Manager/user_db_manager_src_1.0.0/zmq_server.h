/*---------------------- Copyright (c) Picarro, 2017 -------------------
 *                                 Source code
 * This software is the property of Picarro and should be considered and treated
 * as proprietary information.  Refer to the "Source Code License Agreement"
 *----------------------------------------------------------------------------
*/

#include <unistd.h>
#include <zmq.h>
#include <json-c/json.h>


#include "file_parser.h"
#include "db_management.h"
#include "debug.h"


void remove_first_and_last_char(const char *, char *);
void reset_all_buffer_values(void);
json_object *make_array(int );
void print_json_value(json_object *);
void json_parse_array( json_object *, char *);
void get_json_key_value(const char *, const char *);
static char* execute_login_user_command(void);
static char* execute_logout_user_command(void);
static char* execute_current_user_command(void);
static char* execute_add_user_command(void);
static char* execute_delete_user_command(void);
static char* execute_validation_report_command(void);
static char* execute_list_users_command(void);
char* validate_and_execute_commands(void);
void json_parse(json_object *);
static int sort_fn (const void *, const void *);

void* zmq_server_thread(void *);
