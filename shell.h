#ifndef _SHELL_H_
#define _SHELL_H_

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
 * struct passinfo - contains pseudo-arguements to pass into a function,
 * allowing uniform prototype for function pointer struct
 * @arg: a string generated from getline containing arguements
 * @argv:an array of strings generated from arg
 * @path: a string path for the current command
 * @argc: the argument count
 * @line_count: the error count
 * @err_num: the error code for exit()s
 * @linecount_flag: if on count this line of input
 * @fname: the program filename
 * @env: linked list local copy of environ
 * @environ: custom modified copy of environ from LL env
 * @history: the history node
 * @alias: the alias node
 * @env_changed: on if environ was changed
 * @status: the return status of the last exec'd command
 * @cmd_buf: address of pointer to cmd_buf, on if chaining
 * @cmd_buf_type: CMD_type ||, &&, ;
 * @readfd: the fd from which to read line input
 * @histcount: the history line number count
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
 * struct builtin - contains a builtin string and related function
 * @type: the builtin command flag
 * @func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* loop_shell.c */
int loop_shell(info_t *, char **);
int findBuiltIn(info_t *);
void find_command(info_t *);
void fork_command(info_t *);

/* shell_parser.c */
int is_command(info_t *, char *);
char *duplicates_characters(char *, int, int);
char *find_path_shell(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* errors_shell.c */
void custom_puts(char *);
int custom_putchar(char);
int custom_putfile(char c, int fd);
int custom_putsfile(char *str, int fd);

/* shell_str.c */
int custom_strlen(char *);
int custom_strcmp(char *, char *);
char *custom_starts_with(const char *, const char *);
char *custom_strcat(char *, char *);

/* shell_str1.c */
char *custom_strcpy(char *, char *);
char *custom_strdup(const char *);
void custom_puts(char *);
int custom_putchar(char);

/* shell_exits.c */
char *custom_strncpy(char *, char *, int);
char *custom_strncat(char *, char *, int);
char *custom_strchr(char *, char);

/* shell_tokenizer.c */
char **custom_strtow(char *, char *);
char **custom_strtow2(char *, char);

/* shell_realloc.c */
char *memory_set(char *, char, unsigned int);
void ffree_str(char **);
void *shell_realloc(void *, unsigned int, unsigned int);

/* shell_memory.c */
int free_memory(void **);

/* shell_interactive.c */
int is_interactive_mode(info_t *);
int is_character_delimiter(char, char *);
int is_alpha_character(int);
int custom_atoi(char *);

/* errors_shell1.c */
int custom_erratoi(char *);
void custom_print_error(info_t *, char *);
int custom_print_d(int, int);
char *custom_convert_number(long int, int, int);
void custom_remove_comments(char *);

/* shell_builtin.c */
int custom_exit(info_t *);
int custom_cd(info_t *);
int custom_help(info_t *);

/* shell_builtin1.c */
int custom_history(info_t *);
int custom_alias(info_t *);

/* getline.c */
ssize_t getInput(info_t *);
int custom_Getline(info_t *, char **, size_t *);
void HandlerSigint(int);

/* shell_getinfo.c */
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);

/* shell_environment.c */
char *custom_getenv(info_t *, const char *);
int custom_env(info_t *);
int custom_setenv(info_t *);
int custom_myunsetenv(info_t *);
int populate_env_list(info_t *);

/* getenv.c */
char **get_environ(info_t *);
int _unsetenv(info_t *, char *);
int _setenv(info_t *, char *, char *);

/* shell_history.c */
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

/* shell_lists.c */
list_t *node(list_t **, const char *, int);
list_t *node_end(list_t **, const char *, int);
size_t print_list_string(const list_t *);
int delete_node(list_t **, unsigned int);
void free_list_str(list_t **);

/* shell_lists1.c */
size_t list_len_str(const list_t *);
char **list_to_str(list_t *);
size_t print_list_linked(const list_t *);
list_t *node_starts(list_t *, char *, char);
ssize_t get_node(list_t *, list_t *);

/* shell_vars.c */
int ischain_delimiter(info_t *, char *, size_t *);
void check_chain_stat(info_t *, char *, size_t *, size_t, size_t);
int replace_alias_shell(info_t *);
int replace_variables(info_t *);
int replace_string_shell(char **, char *);

#endif
