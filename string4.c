#include "shell.h"

/**
 * **string_split - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @d: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **string_split(char *str, char *d)
{
	int i, l, f, p, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!d)
		d = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!is_delim(str[i], d) && (is_delim(str[i + 1], d) || !str[i + 1]))
			numwords++;

	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, l = 0; l < numwords; l++)
	{
		while (is_delim(str[i], d))
			i++;
		f = 0;
		while (!is_delim(str[i + f], d) && str[i + f])
			f++;
		s[l] = malloc((f + 1) * sizeof(char));
		if (!s[l])
		{
			for (f = 0; f < l; f++)
				free(s[f]);
			free(s);
			return (NULL);
		}
		for (p = 0; p < f; p++)
			s[l][p] = str[i++];
		s[l][p] = 0;
	}
	s[l] = NULL;
	return (s);
}

/**
 * **string_split2 - splits a string into words
 * @str: the input string
 * @d: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **string_split2(char *str, char d)
{
	int i, l, f, p, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != d && str[i + 1] == d) ||
			(str[i] != d && !str[i + 1]) || str[i + 1] == d)
			numwords++;
	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, l = 0; l < numwords; l++)
	{
		while (str[i] == d && str[i] != d)
			i++;
		f = 0;
		while (str[i + f] != d && str[i + f] && str[i + f] != d)
			f++;
		s[l] = malloc((f + 1) * sizeof(char));
		if (!s[l])
		{
			for (f = 0; f < l; f++)
				free(s[f]);
			free(s);
			return (NULL);
		}
		for (p = 0; p < l; p++)
			s[l][p] = str[i++];
		s[l][p] = 0;
	}
	s[l] = NULL;
	return (s);
}
