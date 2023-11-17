#include "shell.h"

/**
 * custom_exit - exits the shell
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: exits with a given exit status
 * (0) if info->arguments[0] != "exit"
 */
int custom_exit(info_t *info)
{
	int exit_check;

	if (info->argv[1]) /* If there is an exit arguement */
	{
		exit_check = custom_erratoi(info->arguments[1]);
		if (exit_check == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = custom_erratoi(info->arguments[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * custom_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int custom_cd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->arguments[1])
	{
		dir = custom_getenv(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = custom_getenv(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (custom_strcmp(info->arguments[1], "-") == 0)
	{
		if (!custom_getenv(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(custom_getenv(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = custom_getenv(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->arguments[1]);
	if (chdir_ret == -1)
	{
		print_error(info, "can't cd to ");
		_eputs(info->arguments[1]), _eputchar('\n');
	}
	else
	{
		custom_setenv(info, "OLDPWD", custom_getenv(info, "PWD="));
		custom_setenv(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * custom_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0
 */
int custom_help(info_t *info)
{
	char **arg_array;

	arg_array = info->arguments;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}

