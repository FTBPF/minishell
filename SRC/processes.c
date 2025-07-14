/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:20 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/14 17:54:10 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_file_opening(char *str, t_vars *vars, char *infile, int *j)
{
	if (*(ft_strrchr(str, '<') - 1) == '<')
	{
		vars->fd0 = vars->here_doc_fd[*j];
		(*j)++;
	}
	else
		vars->fd0 = open(infile, O_RDONLY);
	free(infile);
}

static void	handle_io(t_vars *vars, char **commands)
{
	if (vars->fd1 != 1)
	{
		dup2(vars->fd1, STDOUT_FILENO);
		close(vars->fd1);
	}
	else if (commands[1])
		dup2(vars->pipe_fd[1], STDOUT_FILENO);
	close(vars->pipe_fd[0]);
	if (vars->fd0 != 0)
	{
		dup2(vars->fd0, STDIN_FILENO);
		close(vars->fd0);
	}
	else if (vars->p0 != 0)
		dup2(vars->p0, STDIN_FILENO);
}

void	execute_command(t_vars *vars, char **commands, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	remove_quotes_from_array(vars->cmd_flags);
	vars->cmd1_path = check_valid_cmd(vars->cmd_flags[0], vars->my_environ);
	if (vars->cmd1_path == NULL)
		exit(127);
	handle_io(vars, commands);
	if (check_if_builtin(vars))
	{
		run_builtin(vars);
		exit(0);
	}
	execve(vars->cmd1_path, vars->cmd_flags, envp);
	perror("execve");
	exit(1);
}

static void	cleanup_temp_file(t_vars *vars)
{
	if (vars->temp != NULL)
	{
		unlink(vars->temp);
		free(vars->temp);
		vars->temp = NULL;
	}
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
	wait(&vars->pid1);
	cleanup_temp_file(vars);
	if (vars->cmd_flags)
		ft_free(vars->cmd_flags);
	vars->cmd_flags = NULL;
}
