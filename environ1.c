#include "shell.h"

/**
 * retrieve_custom_environment - returns the string array copy of our custom environment
 * @cinfo: Structure containing potential arguments.
 * Return: Always 0
*/
char **retrieve_custom_environment(CustomShellInfo *cinfo)
{
    if (!cinfo->custom_environment || cinfo->environment_changed)
    {
        cinfo->custom_environment = convert_list_to_strings(cinfo->custom_env);
        cinfo->environment_changed = 0;
    }
    return (cinfo->custom_environment);
}

/**
 * remove_custom_environment_variable - Remove a custom environment variable
 * @cinfo: Structure containing potential arguments.
 * Return: 1 on delete, 0 otherwise
 * @variable: the string custom environment variable property
*/
int remove_custom_environment_variable(CustomShellInfo *cinfo, char *variable)
{
    list_t *node = cinfo->custom_env;
    size_t index = 0;
    char *p;

    if (!node || !variable)
        return 0;

    while (node)
    {
        p = starts_with(node->str, variable);
        if (p && *p == '=')
        {
            cinfo->environment_changed = delete_node_at_index(&(cinfo->custom_env), index);
            index = 0;
            node = cinfo->custom_env;
            continue;
        }
        node = node->next;
        index++;
    }
    return cinfo->environment_changed;
}

/**
 * add_custom_environment_variable - Initialize a new custom environment variable,
 *                                   or modify an existing one
 * @cinfo: Structure containing potential arguments.
 * @variable: the string custom environment variable property
 * @value: the string custom environment variable value
 * Return: Always 0
*/
int add_custom_environment_variable(CustomShellInfo *cinfo, char *variable, char *value)
{
    char *buffer = NULL;
    list_t *node;
    char *p;

    if (!variable || !value)
        return 0;

    buffer = malloc(custom_strlen(variable) + custom_strlen(value) + 2);
    if (!buffer)
        return 1;

    custom_strcpy(buffer, variable);
    custom_strcat(buffer, "=");
    custom_strcat(buffer, value);

    node = cinfo->custom_env;
    while (node)
    {
        p = starts_with(node->str, variable);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buffer;
            cinfo->environment_changed = 1;
            return 0;
        }
        node = node->next;
    }

    add_list_node_end(&(cinfo->custom_env), buffer, 0);
    free(buffer);
    cinfo->environment_changed = 1;
    return 0;
}

