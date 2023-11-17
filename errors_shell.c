#include "shell.h"

/**
 * custom_puts - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void custom_puts(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		custom_putchar(str[i]);
		i++;
	}
}

/**
 * custom_putchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int custom_putchar(char c)
{
	static int i;
	static char buf[CUSTOM_WRITE_BUF_SIZE];

	if (c == CUSTOM_BUF_FLUSH || i >= CUSTOM_WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != CUSTOM_BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * custom_putfile - writes the character c to given file descriptor
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int custom_putfile(char c, int fd)
{
	static int i;
	static char buf[CUSTOM_WRITE_BUF_SIZE];

	if (c == CUSTOM_BUF_FLUSH || i >= CUSTOM_WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != CUSTOM_BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 * custom_putsfile - prints an input string to a given file descriptor
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int custom_putsfile(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += custom_putfd(*str++, fd);
	}
	return (i);
}
