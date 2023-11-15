#include "shell.h"

/**
 * display_custom_environment - Prints the current custom environment
 * @cdata: Structure containing potential arguments.
 * Return: Always 0
*/
int display_custom_environment(CustomShellData *cdata)
{
    print_linked_list_strings(cdata->custom_env);
    return 0;
}

/**
 * fetch_custom_environment_variable - Gets the value of a custom environment variable
 * @cdata: Structure containing potential arguments.
 * @name: Variable name
 * Return: The value of the variable
*/
char *fetch_custom_environment_variable(CustomShellData *cdata, const char *name)
{
    list_t *node = cdata->custom_env;
    char *value;
    while (node)
    {
        value = starts_with(node->str, name);
        if (value && *value)
            return value;
        node = node->next;
    }
    return NULL;
}

/**
 * set_custom_environment_variable - Initializes a new custom environment variable,
 *                                   or modifies an existing one
 * @cdata: Structure containing potential arguments.
 * Return: Always 0
*/
int set_custom_environment_variable(CustomShellData *cdata)
{
    if (cdata->argument_count != 3)
    {
        custom_error_puts("Incorrect number of arguments\n");
        return 1;
    }
    if (set_custom_env_variable(cdata, cdata->arguments[1], cdata->arguments[2]))
        return 0;
    return 1;
}

/**
 * unset_custom_environment_variable - Removes a custom environment variable
 * @cdata: Structure containing potential arguments.
 * Return: Always 0
*/
int unset_custom_environment_variable(CustomShellData *cdata)
{
    int i;
    if (cdata->argument_count == 1)
    {
        custom_error_puts("Too few arguments.\n");
        return 1;
    }
    for (i = 1; i <= cdata->argument_count; i++)
        unset_custom_env_variable(cdata, cdata->arguments[i]);
    return 0;
}

/**
 * initialize_custom_environment_list - Populates the custom environment linked list
 * @cdata: Structure containing potential arguments.
 * Return: Always 0
*/
int initialize_custom_environment_list(CustomShellData *cdata)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; cdata->environment_variables[i]; i++)
        add_node_end(&node, cdata->environment_variables[i], 0);
    cdata->custom_env = node;
    return 0;
}

