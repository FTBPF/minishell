/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:22:05 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 14:41:43 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_pipe_read_if_needed(t_vars *vars, char **commands)
{
	if (commands[1])
		close(vars->pipe_fd[0]);
}

// In the child process:
//  * Sets up default signal handlers
//  * Redirects stdin from fd0 (if set) or prev_read_fd
//  * Redirects stdout to fd1 (if set) or pipe_fd[1]
//  * Closes unused file descriptors
// Exits with status 1 if fd0 is -1 (redirection error).

static void	prepare_child_io(t_vars *vars, int prev_read_fd, char **commands)
{
	signal(SIGINT, SIG_DFL);
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

// If there's a next command, creates a pipe. Then forks a child
// process and stores the PID in vars->pid1.

static int	setup_pipe_and_fork(t_vars *vars, char **commands)
{
	if (commands[1] && pipe(vars->pipe_fd) < 0)
	{
		g_exit_status = 1;
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

//  In the parent process:
//  * Closes previous read fd
//  * Closes custom fd1 and fd0
//  * If more commands, closes write end and stores read end
//  * Cleans up temporary heredoc files

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

// Main function for executing a command:
//  * Initializes file descriptors
//  * Splits command into arguments
//  * Sets up input/output redirections
//  * Creates pipe and forks
//  * In child: sets up I/O and executes
//  * In parent: cleans up resources

void	first_process(t_vars *vars, char **envp, char **commands, int *j)
{
	int				prev_read_fd;

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
	handle_parent_cleanup(vars, prev_read_fd, commands);
}
