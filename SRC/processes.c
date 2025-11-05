/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:31 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:52:28 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_error(t_vars *vars, char **commands)
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
		*exit_status() = 126;
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd(": No such file or directory\n", 2);
		*exit_status() = 127;
	}
	else
	{
		ft_putstr_fd(": Execution failed\n", 2);
		*exit_status() = 1;
	}
	exit(*exit_status());
}

void	cleanup_child_and_exit(t_vars *vars, int exit_code)
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

void	free_all_commands(t_vars *vars)
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
