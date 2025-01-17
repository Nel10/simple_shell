#include "shell.h"
/**
 * main - main function for the shell
 * @argc: number of arguments passed to main
 * @argv: array of arguments passed to main
 * @environment: array of environment variables
 *
 * Return: 0 or exit status, or ?
 */
int main(int argc __attribute__((unused)), char **argv, char **environment)
{
	char *prompt = "$ ";
	size_t len_buffer = 0;
	unsigned int pipeline = 0, iter;
	vars_t vars = {NULL, NULL, NULL, NULL, 0, NULL};

	vars.argv = argv;
	vars.env = make_env(environment);
	if (!isatty(STDIN_FILENO))
		pipeline = 1;
	if (pipeline == 0)
		_puts(prompt);
	while (getline(&(vars.buffer), &len_buffer, stdin) != -1)
	{
		vars.commands = _tokenizer(vars.buffer, ";");
		for (iter = 0; vars.commands && vars.commands[iter] != NULL; iter++)
		{
			vars.av = _tokenizer(vars.commands[iter], "\n \t\r");
			if (vars.av && vars.av[0])
				if (get_comd_fun(&vars) == NULL)
					_path(&vars);
			free(vars.av);
		}
		free(vars.buffer);
		free(vars.commands);
		if (pipeline == 0)
			_puts(prompt);
		vars.buffer = NULL;
	}

	if (pipeline == 0)
		_puts("\n");
	free_env(vars.env);
	free(vars.buffer);
	exit(0);
}
