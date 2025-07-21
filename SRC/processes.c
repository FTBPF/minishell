/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:20 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/21 15:10:54 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_file_opening(char *str, t_vars *vars, char *infile, int *j)
{
	char	*cleaned_filename;

	if (*(ft_strrchr(str, '<') - 1) == '<')
	{
		vars->fd0 = vars->here_doc_fd[*j];
		(*j)++;
	}
	else
	{
		cleaned_filename = remove_quotes_from_string(ft_strdup(infile));
		vars->infile_name = cleaned_filename;
		vars->fd0 = open(cleaned_filename, O_RDONLY);
		if (vars->fd0 == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cleaned_filename, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			g_exit_status = 1;
			vars->redirection_failed = true;
		}
		free(infile);
	}
}

static void	handle_io(t_vars *vars, char **commands)
{
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
	close(vars->pipe_fd[0]);
	if (vars->fd0 != 0)
	{	
		if (vars->fd0 == -1)
			exit(g_exit_status);
		dup2(vars->fd0, STDIN_FILENO);
		close(vars->fd0);
	}
	else if (vars->p0 != 0)
	{
		dup2(vars->p0, STDIN_FILENO);
		close(vars->p0);
	}
}

void	execute_command(t_vars *vars, char **commands, char **envp)
{
	struct stat	info;

	if (vars->redirection_failed)
		exit(g_exit_status);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	remove_quotes_from_array(vars->cmd_flags);
	handle_io(vars, commands);
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

void	first_process(t_vars *vars, char **envp, char **commands, int *j)
{
	vars->fd0 = 0;
	vars->fd1 = 1;
	vars->cmd_flags = ft_split_commands_no_redirection(commands[0], " |<>");
	setup_redirections(commands, vars, j);
	vars->pid1 = fork();
	if (vars->pid1 < 0)
		return ;
	if (strcmp("./minishell", commands[0]) == 0)
		signal(SIGQUIT, SIG_IGN);
	else
		signal(SIGQUIT, handler_quit);
	signal(SIGINT, handler_quit_ctrlc);
	if (vars->pid1 == 0)
		execute_command(vars, commands, envp);
	first_process_helper(vars);
	vars->p0 = vars->pipe_fd[0];
	cleanup_temp_file(vars);
}
