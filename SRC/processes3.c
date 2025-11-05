/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:41 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 17:09:15 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_pipe_read_if_needed(t_vars *vars, char **commands)
{
	if (commands[1])
		close(vars->pipe_fd[0]);
}

static void	prepare_child_io(t_vars *vars, int prev_read_fd, char **commands)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (vars->fd0 != 0)
	{
		if (vars->fd0 == -1)
			exit(1);
		dup2(vars->fd0, STDIN_FILENO);
		close(vars->fd0);
	}
	else if (prev_read_fd != 0)
	{
		dup2(prev_read_fd, STDIN_FILENO);
		close(prev_read_fd);
	}
	if (vars->fd1 != 1)
	{
		dup2(vars->fd1, STDOUT_FILENO);
		close(vars->fd1);
	}
	else if (commands[1])
	{
		dup2(vars->pipe_fd[1], STDOUT_FILENO);
		close(vars->pipe_fd[1]);
	}
	close_pipe_read_if_needed(vars, commands);
}

static int	setup_pipe_and_fork(t_vars *vars, char **commands)
{
	if (commands[1] && pipe(vars->pipe_fd) < 0)
	{
		*exit_status() = 1;
		perror("pipe");
		return (-1);
	}
	vars->pid1 = fork();
	if (vars->pid1 < 0)
	{
		perror("fork");
		return (-1);
	}
	return (0);
}

static void	handle_parent_cleanup(t_vars *vars, int prev_read_fd,
		char **commands)
{
	if (prev_read_fd != 0)
		close(prev_read_fd);
	if (vars->fd1 != 1)
		close(vars->fd1);
	if (vars->fd0 != 0)
		close(vars->fd0);
	if (commands[1])
	{
		close(vars->pipe_fd[1]);
		vars->p0 = vars->pipe_fd[0];
	}
	cleanup_temp_file(vars);
}

void	first_process(t_vars *vars, char **envp, char **commands, int *j)
{
	int	prev_read_fd;

	prev_read_fd = vars->p0;
	vars->fd0 = 0;
	vars->fd1 = 1;
	vars->cmd_flags = ft_split_commands_no_redirection(commands[0], " |<>");
	setup_redirections(commands, vars, j);
	if (setup_pipe_and_fork(vars, commands) < 0)
		return ;
	if (vars->pid1 == 0)
	{
		prepare_child_io(vars, prev_read_fd, commands);
		execute_command(vars, commands, envp);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	handle_parent_cleanup(vars, prev_read_fd, commands);
}
