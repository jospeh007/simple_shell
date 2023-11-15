#include "shell.h"

/**
 * *_my_memset - fills memory with a constant byte
 * @dest: the pointer to the memory area
 * @byte: the byte to fill dest with
 * @n: the amount of bytes to be filled
 * Return: (dest) a pointer to the memory area dest
 */
char *_my_memset(char *dest, char byte, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		dest[i] = byte;
	return (dest);
}

/**
 * my_free - frees a string of strings
 * @str_array: array of strings
 */
void my_free(char **str_array)
{
	char **a = str_array;

	if (!str_array)
		return;
	while (*str_array)
		free(*str_array++);
	free(a);
}

/**
 * *_my_realloc - reallocates a block of memory
 * @ptr: pointer to the previous allocated block
 * @old_size: byte size of previous block
 * @new_size: byte size of new block
 *
 * Return: pointer to the reallocated block
 */
void *_my_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	p = malloc(new_size);
	if (!p)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (p);
}
