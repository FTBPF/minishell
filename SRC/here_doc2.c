/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:04 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/28 17:59:20 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_temp_write_fd(t_vars *vars)
{
	int	fd;

	fd = open(vars->temp, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd == -1)
	{
		perror(vars->temp);
		vars->redirection_failed = true;
	}
	return (fd);
}

static int	fork_heredoc(t_vars *vars, char *doc_file, int write_fd, int expand)
{
	int	id;

	setup_heredoc_parent_signals();
	id = fork();
	if (id == 0)
	{
		setup_heredoc_signals();
		process_heredoc_expanded(vars, doc_file, write_fd, expand);
	}
	return (id);
}

static void	check_final_fd(t_vars *vars, int *j)
{
	if (vars->here_doc_fd[*j] == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(vars->temp);
		vars->redirection_failed = true;
	}
}

void	open_doc_file_expanded(t_vars *vars, char *doc_file, int *j, int expand)
{
	int	id;
	int	write_fd;
	int	status;

	write_fd = open_temp_write_fd(vars);
	if (vars->redirection_failed)
		return (free(doc_file));
	id = fork_heredoc(vars, doc_file, write_fd, expand);
	if (id == -1)
		return ((void)(perror("fork"), close(write_fd), free(doc_file),
			vars->redirection_failed = true));
	close(write_fd);
	waitpid(id, &status, 0);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_DFL);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g_exit_status = 130;
		vars->redirection_failed = true;
		unlink(vars->temp);
	}
	free(doc_file);
	if (!vars->redirection_failed)
		vars->here_doc_fd[*j] = open(vars->temp, O_RDONLY);
	check_final_fd(vars, j);
}

void	ft_open_helper(int *i, char *commands)
{
	char	current_quote;
	int		in_quotes;

	in_quotes = -1;
	current_quote = '\0';
	while (commands[*i])
	{
		if (in_quotes == -1 && (commands[*i] == ' ' || commands[*i] == '<'
				|| commands[*i] == '>'))
			break ;
		if ((commands[*i] == '\'' || commands[*i] == '\"') && (in_quotes == -1
				|| current_quote == commands[*i]))
		{
			in_quotes *= -1;
			if (in_quotes == 1)
				current_quote = commands[*i];
			else
				current_quote = '\0';
		}
		(*i)++;
	}
}
