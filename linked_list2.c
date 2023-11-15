#include "shell.h"

/**
 * custom_list_len - determines length of the linked list
 * @h: pointer to the first node
 *
 * Return: size of list
 */
size_t custom_list_len(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * **custom_list_to_strings - returns an array of strings of the list->str
 * @list_head: pointer to the first node
 *
 * Return: array of strings
 */
char **custom_list_to_strings(list_t *list_head)
{
	list_t *node = list_head;
	size_t i = list_len(list_head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * custom_print_list - prints all elements of a list_t linked list
 * @h: pointer to the first node
 *
 * Return: size of list
 */
size_t custom_print_list(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_custom_puts(convert_number(h->num, 10, 0));
		_custom_putchar(':');
		_custom_putchar(' ');
		_custom_puts(h->str ? h->str : "(nil)");
		_custom_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * custom_node_starts_with - returns node whose string starts with prefix
 * @node_head: pointer to the list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or NULL
 */
list_t *custom_node_starts_with(list_t *node_head, char *prefix, char c)
{
	char *p = NULL;

	while (node_head)
	{
		p = starts_with(node_head->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node_head);
		node_head = node_head->next;
	}
	return (NULL);
}

/**
 * custom_get_node_index - gets the index of a node
 * @list_head: pointer to list head
 * @target_node: pointer to the target node
 *
 * Return: index of node or -1
 */
ssize_t custom_get_node_index(list_t *list_head, list_t *target_node)
{
	size_t i = 0;

	while (list_head)
	{
		if (list_head == target_node)
			return (i);
		list_head = list_head->next;
		i++;
	}
	return (-1);
}
