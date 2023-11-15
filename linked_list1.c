#include "shell.h"

/**
 * add_list_node - adds a node to the start of linked the list
 * @head: address of pointer to head node
 * @str_value: str field of node
 * @index_value: index used by history for the node
 *
 * Return: size of the linked list
 */
list_t *add_list_node(list_t **head, const char *str_value, int index_value)
{
	list_t *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(list_t));
	if (!new_head)
		return (NULL);
	_memset((void *)new_head, 0, sizeof(list_t));
	new_head->index_value = index_value;
	if (str_value)
	{
		new_head->str_value = _custom_strdup(str_value);
		if (!new_head->str_value)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_list_node_end - adds a node to the end of the linked list
 * @head: address of pointer to head node
 * @str_value: value for the str field of node
 * @ind_value: index used by history for the node
 *
 * Return: size of the linked list
 */
list_t *add_list_node_end(list_t **head, const char *str_value, int ind_value)
{
	list_t *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(list_t));
	if (!new_node)
		return (NULL);
	_memset((void *)new_node, 0, sizeof(list_t));
	new_node->ind_value = ind_value;
	if (str_value)
	{
		new_node->str_value = _custom_strdup(str_value);
		if (!new_node->str_value)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * print_list_values - prints only the str_value element of a list_t
 * @h: pointer to the first node
 *
 * Return: size of the linked list
 */
size_t print_list_values(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delete_node_at_position - deletes a node at the given position
 * @head: address of pointer to the first node
 * @position: position of the node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int delete_node_at_position(list_t **head, unsigned int position)
{
	list_t *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!position)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == position)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_linked_list - frees all nodes of a linked list
 * @head_ptr: address of pointer to head node
 *
 * Return: void
 */
void free_linked_list(list_t **head_ptr)
{
	list_t *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
