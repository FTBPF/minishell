/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:20 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 12:18:07 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_file_opening(char *str, t_vars *vars, char *infile, int *j)
{
	(void)infile;
	if (*(ft_strrchr(str, '<') - 1) == '<')
	{
		vars->fd0 = vars->here_doc_fd[*j];
		(*j)++;
	}
}

void	execute_command(t_vars *vars, char **commands, char **envp)
{
	struct stat	info;

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
	{
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
}

static void	cleanup_temp_file(t_vars *vars)
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

// FIXED FIRST_PROCESS FUNCTION
void	first_process(t_vars *vars, char **envp, char **commands, int *j)
{
	int	prev_read_fd;

	prev_read_fd = vars->p0;
	vars->fd0 = 0;
	vars->fd1 = 1;
	vars->cmd_flags = ft_split_commands_no_redirection(commands[0], " |<>");
	setup_redirections(commands, vars, j);
	if (commands[1])
	{
		if (pipe(vars->pipe_fd) < 0)
		{
			g_exit_status = 1;
			perror("pipe");
			return ;
		}
	}
	vars->pid1 = fork();
	if (vars->pid1 < 0)
	{
		perror("fork");
		return ;
	}
	if (vars->pid1 == 0)
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
		if (commands[1])
			close(vars->pipe_fd[0]);
		execute_command(vars, commands, envp);
	}
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
