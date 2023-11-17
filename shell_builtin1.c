#include "shell.h"

/**
 * custom_history - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int custom_history(info_t *info)
{
	print_list(info->command_history);
	return (0);
}

/**
 * unset_custom_alias - remove an alias from the list
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_custom_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = custom_strchr(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(info->aliases),
		get_node_index(info->aliases, node_starts_with(info->aliases, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_custom_alias - sets an alias to a string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_custom_alias(info_t *info, char *str)
{
	char *p;

	p = custom_strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(info, str));

	unset_custom_alias(info, str);
	return (add_node_end(&(info->aliases), str, 0) == NULL);
}

/**
 * print_custom_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_custom_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = custom_strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
		custom_putchar(*a);
		custom_putchar('\'');
		custom_puts(p + 1);
		custom_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * custom_alias -  manages aliases similar to the alias builtin
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int custom_alias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->aliases;
		while (node)
		{
			print_custom_aliases(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = custom_strchr(info->argv[i], '=');
		if (p)
			set_custom_alias(info, info->argv[i]);
		else
			print_custom_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
