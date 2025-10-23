/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:31 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/23 14:04:57 by frteixei         ###   ########.fr       */
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

void	execute_command(t_vars *vars, char **commands, char **envp)
{
	if (vars->redirection_failed)
		exit(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	remove_quotes_from_array(vars->cmd_flags);
	if (check_if_builtin(vars))
	{
		run_builtin(vars);
		exit(g_exit_status);
	}
	vars->cmd1_path = check_valid_cmd(vars->cmd_flags[0], vars->my_environ);
	if (vars->cmd1_path == NULL)
	{
		g_exit_status = 127;
		exit(g_exit_status);
	}
	if (execve(vars->cmd1_path, vars->cmd_flags, envp) == -1)
		execute_error(vars, commands);
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
