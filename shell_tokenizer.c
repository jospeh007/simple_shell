#include "shell.h"

/**
 * **custom_strtow - splits a string into words. Repeat delimiters are ignored
 * @str: the input string
 * @delimiters: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */

char **custom_strtow(char *str, char *delimiters)
{
	int i, j, k, m, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	if (!delimiters)
		d = " ";
	for (i = 0; str[i] != '\0'; i++)
		if (!is_delim(str[i], delimiters) && (is_delim(str[i + 1],
						delimiters) || !str[i + 1]))
			numwords++;

	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (is_delim(str[i], delimiters))
			i++;
		k = 0;
		while (!is_delim(str[i + k], delimiters) && str[i + k])
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;
	return (s);
}

/**
 * **custom_strtow2 - splits a string into words
 * @str: the input string
 * @delimiters: the delimeter
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **custom_strtow2(char *str, char delimiters)
{
	int i, j, k, m, numwords = 0;
	char **s;

	if (str == NULL || str[0] == 0)
		return (NULL);
	for (i = 0; str[i] != '\0'; i++)
		if ((str[i] != delimiters && str[i + 1] == delimiters) ||
				    (str[i] != d && !str[i + 1]) || str[i + 1] == delimiters)
			numwords++;
	if (numwords == 0)
		return (NULL);
	s = malloc((1 + numwords) * sizeof(char *));
	if (!s)
		return (NULL);
	for (i = 0, j = 0; j < numwords; j++)
	{
		while (str[i] == delimiters && str[i] != delimiters)
			i++;
		k = 0;
		while (str[i + k] != delimiters && str[i + k] && str[i + k] != delimiters)
			k++;
		s[j] = malloc((k + 1) * sizeof(char));
		if (!s[j])
		{
			for (k = 0; k < j; k++)
				free(s[k]);
			free(s);
			return (NULL);
		}
		for (m = 0; m < k; m++)
			s[j][m] = str[i++];
		s[j][m] = 0;
	}
	s[j] = NULL;
	return (s);
}
