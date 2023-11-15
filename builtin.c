#include "shell.h"

/**
 * exit_shell - Exits the shell
 * @cinfo: Structure containing potential arguments.
 * Return: Exits with a given exit status
 *         (0) if cinfo->argument_vector[0] != "exit"
*/
int exit_shell(CustomShellInfo *cinfo)
{
    int exit_code;

    if (cinfo->argument_vector[1])
    {
        exit_code = custom_atoi(cinfo->argument_vector[1]);
        if (exit_code == -1)
        {
            cinfo->exit_status = 2;
            print_error(cinfo, "Illegal number: ");
            custom_error_puts(cinfo->argument_vector[1]);
            custom_error_putchar('\n');
            return 1;
        }
        cinfo->error_number = custom_atoi(cinfo->argument_vector[1]);
        return -2;
    }

    cinfo->error_number = -1;
    return -2;
}

/**
 * change_directory - Changes the current directory of the process
 * @cinfo: Structure containing potential arguments.
 * Return: Always 0
*/
int change_directory(CustomShellInfo *cinfo)
{
    char *current_directory, *new_directory, buffer[1024];
    int chdir_result;

    current_directory = getcwd(buffer, 1024);
    
    if (!current_directory)
        custom_puts("TODO: >>getcwd failure message here<<\n");

    if (!cinfo->argument_vector[1])
    {
        new_directory = fetch_custom_environment_variable(cinfo, "HOME=");
        if (!new_directory)
            chdir_result = chdir((new_directory = fetch_custom_environment_variable(cinfo, "PWD=")) ? new_directory : "/");
        else
            chdir_result = chdir(new_directory);
    }
    else if (custom_strcmp(cinfo->argument_vector[1], "") == 0)
    {
        if (!fetch_custom_environment_variable(cinfo, "OLDPWD="))
            custom_putchar('\n');
        
        chdir_result = chdir((new_directory = fetch_custom_environment_variable(cinfo, "OLDPWD=")) ? new_directory : "/");
    }
    else
        chdir_result = chdir(cinfo->argument_vector[1]);

    if (chdir_result == -1)
    {
        print_error(cinfo, "can't cd to ");
        custom_error_puts(cinfo->argument_vector[1]);
        custom_error_putchar('\n');
    }
    else
    {
        set_custom_environment_variable(cinfo, "OLDPWD", fetch_custom_environment_variable(cinfo, "PWD="));
        set_custom_environment_variable(cinfo, "PWD", getcwd(buffer, 1024));
    }
    
    return 0;
}

/**
 * display_help - Displays help information
 * @cinfo: Structure containing potential arguments.
 * Return: Always 0
*/
int display_help(CustomShellInfo *cinfo)
{
    char **argument_array;
    
    argument_array = cinfo->argument_vector;
    custom_puts("Help call works. Function not yet implemented\n");

    if (0)
        custom_puts(*argument_array);

    return 0;
}

