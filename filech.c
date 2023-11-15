#include "shell.h"

/**
 * @brief Checks if a character in the buffer is a command delimiter.
 *
 * @param command_info The struct holding command information.
 * @param buffer The character buffer.
 * @param position Address of the current position in the buffer.
 * @return 1 if it's a command delimiter, 0 otherwise.
 */
int is_command_delimiter(CommandInfo *command_info, char *buffer, size_t *position)
{
    size_t current_position = *position;

    if (buffer[current_position] == '|' && buffer[current_position + 1] == '|')
    {
        buffer[current_position] = 0;
        current_position++;
        command_info->buffer_type = CMD_OR;
    }
    else if (buffer[current_position] == '&' && buffer[current_position + 1] == '&')
    {
        buffer[current_position] = 0;
        current_position++;
        command_info->buffer_type = CMD_AND;
    }
    else if (buffer[current_position] == ';') 
    {
        buffer[current_position] = 0; 
        command_info->buffer_type = CMD_CHAIN;
    }
    else
    {
        return 0;
    }

    *position = current_position;
    return 1;
}

/**
 * @brief Checks whether to continue chaining based on the last status.
 *
 * @param command_info The struct holding command information.
 * @param buffer The character buffer.
 * @param position Address of the current position in the buffer.
 * @param start Starting position in the buffer.
 * @param length Length of the buffer.
 */
void check_command_chain(CommandInfo *command_info, char *buffer, size_t *position, size_t start, size_t length)
{
    size_t current_position = *position;

    if (command_info->buffer_type == CMD_AND)
    {
        if (command_info->status)
        {
            buffer[start] = 0;
            current_position = length;
        }
    }
    if (command_info->buffer_type == CMD_OR)
    {
        if (!command_info->status)
        {
            buffer[start] = 0;
            current_position = length;
        }
    }

    *position = current_position;
}

/**
 * @brief Replaces an alias in the tokenized string.
 *
 * @param command_info The struct holding command information.
 * @return 1 if replaced, 0 otherwise.
 */
int replace_command_alias(CommandInfo *command_info)
{
    int i;
    ListNode *node;
    char *substring;

    for (i = 0; i < 10; i++)
    {
        node = find_node_starts_with(command_info->alias_list, command_info->arguments[0], '=');
        if (!node)
            return 0;
        free(command_info->arguments[0]);
        substring = custom_strchr(node->str, '=');
        if (!substring)
            return 0;
        substring = _strdup(substring + 1);
        if (!substring)
            return 0;
        command_info->arguments[0] = substring;
    }
    return 1;
}

/**
 * @brief Replaces variables in the tokenized string.
 *
 * @param command_info The struct holding command information.
 * @return 1 if replaced, 0 otherwise.
 */
int replace_command_vars(CommandInfo *command_info)
{
    int i = 0;
    ListNode *node;

    for (i = 0; command_info->arguments[i]; i++)
    {
        if (command_info->arguments[i][0] != '$' || !command_info->arguments[i][1])
            continue;

        if (!custom_strcmp(command_info->arguments[i], "$?"))
        {
            replace_string(&(command_info->arguments[i]), _strdup(convert_number(command_info->status, 10, 0)));
            continue;
        }
        if (!custom_strcmp(command_info->arguments[i], "$$"))
        {
            replace_string(&(command_info->arguments[i]), _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = find_node_starts_with(command_info->env_list, &command_info->arguments[i][1], '=');
        if (node)
        {
            replace_string(&(command_info->arguments[i]), _strdup(custom_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&command_info->arguments[i], _strdup(""));
    }
    return 0;
}

/**
 * @brief Replaces a string.
 *
 * @param old_address Address of the old string.
 * @param new_string New string.
 * @return 1 if replaced, 0 otherwise.
 */
int replace_string(char **old_address, char *new_string)
{
    free(*old_address);
    *old_address = new_string;
    return 1;
}
