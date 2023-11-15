#include "shell.h"

/**
 * @brief Display the file content.
 *
 * @param file_info The structure containing information about the file.
 * @return Always returns 0.
 */
int display_file_content(FileInfo *file_info)
{
    print_list(file_info->file_lines);
    return 0;
}

/**
 * @brief Remove an entry from the file list.
 *
 * @param file_info The structure containing information about the file.
 * @param entry The entry to be removed.
 * @return Always returns 0 on success, 1 on error.
 */
int remove_file_entry(FileInfo *file_info, char *entry)
{
    char *delimiter_position, temp_char;
    int result;

    delimiter_position = custom_strchr(entry, '=');

    if (!delimiter_position)
        return 1;

    temp_char = *delimiter_position;
    *delimiter_position = 0;

    result = delete_node_at_index(&(file_info->file_entries),
                                   get_node_index(file_info->file_entries, node_starts_with(file_info->file_entries, entry, -1)));

    *delimiter_position = temp_char;
    return result;
}

/**
 * @brief Add an entry to the file list.
 *
 * @param file_info The structure containing information about the file.
 * @param entry The entry to be added.
 * @return Always returns 0 on success, 1 on error.
 */
int add_file_entry(FileInfo *file_info, char *entry)
{
    char *delimiter_position;

    delimiter_position = custom_strchr(entry, '=');

    if (!delimiter_position)
        return 1;

    if (!*++delimiter_position)
        return remove_file_entry(file_info, entry);

    remove_file_entry(file_info, entry);
    return add_node_end(&(file_info->file_entries), entry, 0) == NULL;
}

/**
 * @brief Print a single file entry.
 *
 * @param file_node The node containing file entry information.
 * @return Always returns 0 on success, 1 on error.
 */
int print_single_file_entry(list_t *file_node)
{
    char *delimiter_position = NULL, *entry_part = NULL;

    if (file_node)
    {
        delimiter_position = custom_strchr(file_node->str, '=');

        for (entry_part = file_node->str; entry_part <= delimiter_position; entry_part++)
            custom_putchar(*entry_part);

        custom_putchar('\'');
        custom_puts(delimiter_position + 1);
        custom_puts("'\n");

        return 0;
    }

    return 1;
}

/**
 * @brief Manage file entries, mimicking a file system.
 *
 * @param file_info The structure containing information about the file.
 * @return Always returns 0.
 */
int manage_file_entries(FileInfo *file_info)
{
    int i = 0;
    char *delimiter_position = NULL;
    list_t *file_node = NULL;

    if (file_info->entry_count == 1)
    {
        file_node = file_info->file_entries;

        while (file_node)
        {
            print_single_file_entry(file_node);
            file_node = file_node->next;
        }

        return 0;
    }

    for (i = 1; file_info->entry_vector[i]; i++)
    {
        delimiter_position = custom_strchr(file_info->entry_vector[i], '=');

        if (delimiter_position)
            add_file_entry(file_info, file_info->entry_vector[i]);
        else
            print_single_file_entry(node_starts_with(file_info->file_entries, file_info->entry_vector[i], '='));
    }

    return 0;
}

