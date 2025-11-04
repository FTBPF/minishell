/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:31 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/04 12:36:52 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_error(t_vars *vars, char **commands)
{
	struct stat	info;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(commands[0], 2);
	if (errno == EACCES)
	{
		if (stat(vars->cmd1_path, &info) == 0 && S_ISDIR(info.st_mode))
			ft_putstr_fd(": Is a directory\n", 2);
		else
			ft_putstr_fd(": Permission denied\n", 2);
		g_exit_status = 126;
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd(": No such file or directory\n", 2);
		g_exit_status = 127;
	}
	else
	{
		ft_putstr_fd(": Execution failed\n", 2);
		g_exit_status = 1;
	}
	exit(g_exit_status);
}

static void	cleanup_child_and_exit(t_vars *vars, int exit_code)
{
	if (vars->all_commands)
	{
		ft_free(vars->all_commands);
		vars->all_commands = NULL;
	}
	if (vars->my_environ)
		ft_free(vars->my_environ);
	ft_free_vars_in_child(vars);
	exit(exit_code);
}

static void	free_all_commands(t_vars *vars)
{
	int	i;

	if (vars->all_commands)
	{
		i = 0;
		while (vars->all_commands[i])
		{
			free(vars->all_commands[i]);
			vars->all_commands[i++] = NULL;
		}
		free(vars->all_commands);
		vars->all_commands = NULL;
	}
}

void	execute_command(t_vars *vars, char **commands, char **envp)
{
	if (vars->redirection_failed)
		cleanup_child_and_exit(vars, 1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	remove_quotes_from_array(vars->cmd_flags);
	if (check_if_builtin(vars))
	{
		run_builtin(vars);
		free_all_commands(vars);
		cleanup_child_and_exit(vars, g_exit_status);
	}
	vars->cmd1_path = check_valid_cmd(vars->cmd_flags[0], vars->my_environ);
	if (vars->cmd1_path == NULL)
	{
		free_all_commands(vars);
		cleanup_child_and_exit(vars, 127);
	}
	if (vars->my_environ)
	{
		ft_free(vars->my_environ);
		vars->my_environ = NULL;
	}
	if (vars->here_doc_fd)
	{
		free(vars->here_doc_fd);
		vars->here_doc_fd = NULL;
	}
	if (vars->temp)
	{
		free(vars->temp);
		vars->temp = NULL;
	}
	if (execve(vars->cmd1_path, vars->cmd_flags, envp) == -1)
	{
		execute_error(vars, commands);
		if (vars->cmd1_path)
		{
			free(vars->cmd1_path);
			vars->cmd1_path = NULL;
		}
		cleanup_child_and_exit(vars, g_exit_status);
	}
}

void	cleanup_temp_file(t_vars *vars)
{
	if (vars->temp != NULL)
	{
		unlink(vars->temp);
		free(vars->temp);
		vars->temp = NULL;
	}
	if (vars->cmd_flags)
		ft_free(vars->cmd_flags);
	vars->cmd_flags = NULL;
	if (vars->cmd1_path)
	{
		free(vars->cmd1_path);
		vars->cmd1_path = NULL;
	}
}

char	*find_unquoted_char(char *str, char c)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		if (*str == '\'' || *str == '"')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = *str;
			}
			else if (*str == quote_char)
			{
				in_quotes = 0;
				quote_char = '\0';
			}
		}
		else if (!in_quotes && *str == c)
			return (str);
		str++;
	}
	return (NULL);
}
