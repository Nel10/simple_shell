#include "shell.h"

int check_for_dir(char *str)
{
	unsigned int i;

	for (i = 0; str[i]; i++)
	{
		if (str[i] == '/')
			return (1);
	}
	return (0);
}

int exe_path_dir(vars_t *vars)
{
	pid_t pid;
	struct stat buff;

	if (stat(vars->av[0], &buff) == 0)
	{
		if (access(vars->av[0], X_OK) == 0)
		{
			pid = fork();
			if (pid == 0)
				execve(vars->av[0], vars->av, vars->env);
			else
			{
				wait(&vars->status);
				return (0);
			}
		}
		else
		{
			vars->status = 127;
			return (1);
		}
   	}
	return (0);
}

void _path(vars_t *vars)
{
	unsigned int i = 0, iter;
	char *path, *path_dup = NULL, *check_path = NULL;
	char **path_token = NULL;
	struct stat buff;

	if (check_for_dir(vars->av[0]))
		i = exe_path_dir(vars);
	else
	{
		path = find_path(vars->env);
		if (path != NULL)
		{
			path_dup = _strdup(path);
			path_token = _tokenizer(path_dup, ":");
			for (iter = 0; path_token[iter]; iter++)
			{
				check_path = _strcat(path_token[iter], vars->av[0]);
				if (stat(check_path, &buff) == 0)
				{
					i = path_execute(check_path, vars);
					free(check_path);
					break;
				}
			}
			free(path_dup);
		}
	}
	if (i == 1)
		get_exit(vars);
}
