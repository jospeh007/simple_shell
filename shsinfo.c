#include "shell.h"

/**
 * init_shell_info - initializes CustomShellInfo struct
 * @cinfo: struct address
*/
void init_shell_info(CustomShellInfo *cinfo) 
{
    cinfo->argument = NULL;
    cinfo->argument_vector = NULL;
    cinfo->executable_path = NULL;
    cinfo->argument_count = 0;
}

/**
 * configure_shell_info - configures CustomShellInfo struct
 * @cinfo: struct address
 * @args: argument vector
*/
void configure_shell_info(CustomShellInfo *cinfo, char **args)
{
    int i = 0;

    cinfo->file_name = args[0];
    if (cinfo->argument)
    {
        cinfo->argument_vector = strtow(cinfo->argument, " \t");
        if (!cinfo->argument_vector)
        {
            cinfo->argument_vector = malloc(sizeof(char *) * 2);
            if (cinfo->argument_vector)
            {
                cinfo->argument_vector[0] = _strdup(cinfo->argument);
                cinfo->argument_vector[1] = NULL;
            }
        }
        for (i = 0; cinfo->argument_vector && cinfo->argument_vector[i]; i++);
        cinfo->argument_count = i;
        replace_alias(cinfo);
        replace_variables(cinfo);
    }
}

/**
 * release_shell_info - releases CustomShellInfo struct fields
 * @cinfo: struct address
 * @free_all: true if freeing all fields
*/
void release_shell_info(CustomShellInfo *cinfo, int free_all)
{
    ffree(cinfo->argument_vector);
    cinfo->argument_vector = NULL;
    cinfo->executable_path = NULL;
    
    if (free_all)
    {
        if (!cinfo->command_buffer)
            free(cinfo->argument);
        
        if (cinfo->environment)
            free_list(&(cinfo->environment));
        
        if (cinfo->history)
            free_list(&(cinfo->history));
        
        if (cinfo->alias)
            free_list(&(cinfo->alias));
        
        ffree(cinfo->environment_variables);
        cinfo->environment_variables = NULL;
        
        bfree((void **)cinfo->command_buffer);
        
        if (cinfo->read_file_descriptor > 2)
            close(cinfo->read_file_descriptor);
        
        _putchar(BUFFER_FLUSH);
    }
}

