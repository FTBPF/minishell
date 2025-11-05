/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marada <marada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:45:52 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 17:59:01 by marada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_builtin_child(t_vars *vars)
{
	run_builtin(vars);
	free_all_commands(vars);
	cleanup_child_and_exit(vars, *exit_status());
}

static void	handle_exec_error(t_vars *vars, char **commands)
{
	execute_error(vars, commands);
	if (vars->cmd1_path)
	{
		free(vars->cmd1_path);
		vars->cmd1_path = NULL;
	}
	if (vars->my_environ)
	{
		ft_free(vars->my_environ);
		vars->my_environ = NULL;
	}
	cleanup_child_and_exit(vars, *exit_status());
}

static void	free_doc_temp(t_vars *vars)
{
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
}

void	execute_command(t_vars *vars, char **commands, char **envp)
{
	(void)envp;
	if (vars->redirection_failed)
		cleanup_child_and_exit(vars, 1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	remove_quotes_from_array(vars->cmd_flags);
	if (check_if_builtin(vars))
		handle_builtin_child(vars);
	vars->cmd1_path = check_valid_cmd(vars->cmd_flags[0], vars->my_environ);
	if (!vars->cmd1_path)
	{
		free_all_commands(vars);
		cleanup_child_and_exit(vars, 127);
	}
	free_doc_temp(vars);
	if (execve(vars->cmd1_path, vars->cmd_flags, vars->my_environ) == -1)
		handle_exec_error(vars, commands);
}
