#include "shell.h"

/**
 * @brief Checks if a file is an executable command.
 *
 * @param cinfo The custom shell information struct.
 * @param file_path The path to the file.
 * @return 1 if true, 0 otherwise.
 */
int is_executable_command(CustomShellInfo *cinfo, char *file_path)
{
    struct stat file_stat;

    (void)cinfo;

    if (!file_path || stat(file_path, &file_stat))
        return 0;

    if (file_stat.st_mode & S_IFREG)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Duplicates characters from a string.
 *
 * @param path_str The PATH string.
 * @param start_idx The starting index.
 * @param stop_idx The stopping index.
 * @return Pointer to a new buffer.
 */
char *duplicate_characters(char *path_str, int start_idx, int stop_idx)
{
    static char buffer[1024];
    int i = 0, k = 0;

    for (k = 0, i = start_idx; i < stop_idx; i++)
    {
        if (path_str[i] != ':')
        {
            buffer[k++] = path_str[i];
        }
    }

    buffer[k] = '\0';
    return buffer;
}

/**
 * @brief Finds the full path of a command in the PATH string.
 *
 * @param cinfo The custom shell information struct.
 * @param path_str The PATH string.
 * @param command The command to find.
 * @return Full path of the command if found, or NULL.
 */
char *find_command_path(CustomShellInfo *cinfo, char *path_str, char *command)
{
    int i = 0, current_pos = 0;
    char *full_path;

    if (!path_str)
        return NULL;

    if ((custom_strlen(command) > 2) && starts_with(command, "./"))
    {
        if (is_executable_command(cinfo, command))
            return command;
    }

    while (1)
    {
        if (!path_str[i] || path_str[i] == ':')
        {
            full_path = duplicate_characters(path_str, current_pos, i);

            if (!*full_path)
                custom_strcat(full_path, command);
            else
            {
                custom_strcat(full_path, "/");
                custom_strcat(full_path, command);
            }

            if (is_executable_command(cinfo, full_path))
                return full_path;

            if (!path_str[i])
                break;

            current_pos = i;
        }

        i++;
    }

    return NULL;
}

