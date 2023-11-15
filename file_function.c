#include "shell.h"

/**
 * obtain_history_file - obtains the history file
 * @cinfo: parameter struct
 * 
 * Return: allocated string containing history file
*/
char *obtain_history_file(CustomShellInfo *cinfo)
{
    char *buffer, *home_directory;
    home_directory = fetch_custom_environment_variable(cinfo, "HOME=");
    if (!home_directory)
        return NULL;
    
    buffer = malloc(sizeof(char) * (custom_strlen(home_directory) + custom_strlen(HISTORY_FILE) + 2));
    if (!buffer)
        return NULL;
    
    buffer[0] = 0;
    custom_strcpy(buffer, home_directory);
    custom_strcat(buffer, "/");
    custom_strcat(buffer, HISTORY_FILE);
    return buffer;
}

/**
 * record_history - creates a file, or appends to an existing file
 * @cinfo: the parameter struct
 * 
 * Return: 1 on success, else -1
*/
int record_history(CustomShellInfo *cinfo)
{
    ssize_t file_descriptor;
    char *filename = obtain_history_file(cinfo);
    list_t *node = NULL;

    if (!filename)
        return -1;
    
    file_descriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);
    
    if (file_descriptor == -1)
        return -1;

    for (node = cinfo->history; node; node = node->next)
    {
        custom_puts_fd(node->str, file_descriptor);
        custom_put_fd('\n', file_descriptor);
    }

    custom_put_fd(BUFFER_FLUSH, file_descriptor);
    close(file_descriptor);
    return 1;
}

/**
 * load_history - reads history from file
 * @cinfo:  the parameter struct
 * 
 * Return: histcount on success, 0 otherwise
*/
int load_history(CustomShellInfo *cinfo)
{
    int i, last = 0, line_count = 0;
    ssize_t file_descriptor, read_length, file_size = 0;
    struct stat file_stat;
    char *buffer = NULL, *filename = obtain_history_file(cinfo);

    if (!filename)
        return 0;

    file_descriptor = open(filename, O_RDONLY);
    free(filename);

    if (file_descriptor == -1)
        return 0;

    if (!fstat(file_descriptor, &file_stat))
        file_size = file_stat.st_size;

    if (file_size < 2)
        return 0;

    buffer = malloc(sizeof(char) * (file_size + 1));

    if (!buffer)
        return 0;

    read_length = read(file_descriptor, buffer, file_size);
    buffer[file_size] = 0;

    if (read_length <= 0)
        return free(buffer), 0;

    close(file_descriptor);

    for (i = 0; i < file_size; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = 0;
            record_history_entry(cinfo, buffer + last, line_count++);
            last = i + 1;
        }
    }

    if (last != i)
        record_history_entry(cinfo, buffer + last, line_count++);

    free(buffer);
    cinfo->histcount = line_count;

    while (cinfo->histcount-- >= HISTORY_MAX)
        delete_node_at_index(&(cinfo->history), 0);

    renumber_history(cinfo);
    return cinfo->histcount;
}

/**
 * record_history_entry - adds entry to a history linked list
 * @cinfo: Structure containing potential arguments.
 * @buffer: buffer
 * @line_count: the history line count, histcount
 * 
 * Return: Always 0
*/
int record_history_entry(CustomShellInfo *cinfo, char *buffer, int line_count)
{
    list_t *node = NULL;

    if (cinfo->history)
        node = cinfo->history;
    
    add_node_end(&node, buffer, line_count);

    if (!cinfo->history)
        cinfo->history = node;

    return 0;
}

/**
 * renumber_history_entries - renumber the history linked list after changes
 * @cinfo: Structure containing potential arguments.
 * 
 * Return: the new histcount
*/
int renumber_history_entries(CustomShellInfo *cinfo)
{
    list_t *node = cinfo->history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }

    return (cinfo->histcount = i);
}

