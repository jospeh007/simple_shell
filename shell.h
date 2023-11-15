#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* hsh.c */
int hsh(info_t *, char **);
int find_builtin_command(info_t *);
void find_cmd_command(info_t *);
void fork_cmd_command(info_t *);

/* string1.c */
int str_len(char *);
int str_compare(char *, char *);
char *starts_with(const char *, const char *);
char *str_cat(char *, char *);

/* string2.c */
char *str_copy(char *, char *);
char *str_duplicate(const char *);
void print_string(char *);
int write_character(char);

/* string3.c */
char *str_cpy(char *, char *, int);
char *str_cat(char *, char *, int);
char *str_find_char(char *, char);

/* string4.c */
char **string_split(char *str, char *d);
char **string_split2(char *str, char d);

/* memory_shell1.c */
char *_my_memset(char *dest, char byte, unsigned int n);
void my_free(char **pp);
void *_my_realloc(void *, unsigned int, unsigned int);

/* memory_shell2.c */
int my_free_and_null(void **pointer);

/* more_memory_shell.c */
int check_interactive(info_t *shell_info);
int is_delimiter(char character, char *delimiters);
int my_is_alpha(int ch);
int my_atoi(char *str);

/* more_memory_shell2.c */
int safe_atoi(char *str);
void print_custom_error(info_t *info, char *error_message_str);
int print_decimal(int num, int fd);
char *convert_to_string(long int num, int base, int flags);
void eliminate_comments(char *buffer);

/* linked_list1.c */
list_t *add_list_node(list_t **head, const char *str_value, int index_value);
list_t *add_list_node_end(list_t **head, const char *str_value, int ind_value);
size_t print_list_values(const list_t *h);
int delete_node_at_position(list_t **head, unsigned int position);
void free_linked_list(list_t **head_ptr);

/* linked_list2.c */
size_t custom_list_len(const list_t *h);
char **custom_list_to_strings(list_t *list_head);
size_t custom_print_list(const list_t *h);
list_t *custom_node_starts_with(list_t *node_head, char *prefix, char c);
ssize_t custom_get_node_index(list_t *list_head, list_t *target_node);

/* error_string.c */
void print_error_msg(char *msg, int fd);
int write_to_fd(char c, int fd);
int write_to_specific_fd(char c, int fd);
int print_to_specific_fd(char *str, int fd);

/* shsinfo.c */
void init_shell_info(CustomShellInfo *cinfo);
void configure_shell_info(CustomShellInfo *cinfo, char **args);
void release_shell_info(CustomShellInfo *cinfo, int free_all);

/* environ.c*/
int display_custom_environment(CustomShellData *cdata);
char *fetch_custom_environment_variable(CustomShellData *cdata, const char *name);
int set_custom_environment_variable(CustomShellData *cdata);
int unset_custom_environment_variable(CustomShellData *cdata);
int initialize_custom_environment_list(CustomShellData *cdata);

/* environ1.c*/
char **retrieve_custom_environment(CustomShellInfo *cinfo);
int remove_custom_environment_variable(CustomShellInfo *cinfo, char *variable);
int add_custom_environment_variable(CustomShellInfo *cinfo, char *variable, char *value);

/* file.function.c*/
char *obtain_history_file(CustomShellInfo *cinfo);
int record_history(CustomShellInfo *cinfo);
int load_history(CustomShellInfo *cinfo);
int record_history_entry(CustomShellInfo *cinfo, char *buffer, int line_count);
int renumber_history_entries(CustomShellInfo *cinfo);

/* builtin.c*/
int exit_shell(CustomShellInfo *cinfo);
int change_directory(CustomShellInfo *cinfo);
int display_help(CustomShellInfo *cinfo);

/* builtin1.c*/
int display_file_content(FileInfo *file_info);
int remove_file_entry(FileInfo *file_info, char *entry);
int add_file_entry(FileInfo *file_info, char *entry);
int print_single_file_entry(list_t *file_node);
int manage_file_entries(FileInfo *file_info);

/* file.path.c*/
int is_executable_command(CustomShellInfo *cinfo, char *file_path);
char *duplicate_characters(char *path_str, int start_idx, int stop_idx);
char *find_command_path(CustomShellInfo *cinfo, char *path_str, char *command);

/* filech.c*/
int is_command_delimiter(CommandInfo *command_info, char *buffer, size_t *position);
void check_command_chain(CommandInfo *command_info, char *buffer, size_t *position, size_t start, size_t length);
int replace_command_alias(CommandInfo *command_info);
int replace_command_vars(CommandInfo *command_info);
int replace_string(char **old_address, char *new_string);

/* getline1.c*/
ssize_t bufferCommandInput(CommandInfo *commandInfo, char **buffer, size_t *bufferLength);
ssize_t getInput(CommandInfo *commandInfo);
ssize_t readBuffer(CommandInfo *commandInfo, char *buffer, size_t *iterator);
int customGetline(CommandInfo *commandInfo, char **pointer, size_t *length);
void blockCtrlC(__attribute__((unused))int signalNumber);

