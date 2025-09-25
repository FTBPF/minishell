/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:20:08 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 16:20:53 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	get_first_redir_positions(t_redir *redirs, int count,
		int *first_output, int *first_input)
{
	int	i;

	*first_output = -1;
	*first_input = -1;
	i = 0;
	while (i < count)
	{
		if (redirs[i].type == '>' && *first_output == -1)
			*first_output = redirs[i].index;
		if (redirs[i].type == '<' && *first_input == -1)
			*first_input = redirs[i].index;
		i++;
	}
}

bool	output_then_input(const char *str)
{
	int		count;
	t_redir	*redirs;
	bool	result;
	int		first_output;
	int		first_input;

	count = 0;
	redirs = find_redirections(str, &count);
	if (!redirs)
		return (false);
	get_first_redir_positions(redirs, count, &first_output, &first_input);
	result = false;
	if (first_output != -1 && first_input != -1 && first_output < first_input)
		result = true;
	free(redirs);
	return (result);
}

static void	check_heredoc(char **commands, t_vars *vars)
{
	if (has_unquoted_heredoc(commands[0]))
		here_doc(vars, vars->cmd_flags);
}

void	setup_redirections(char **commands, t_vars *vars, int *j)
{
	int		count;
	int		i;
	t_redir	*redirs;

	vars->redirection_failed = false;
	count = 0;
	redirs = find_redirections(commands[0], &count);
	check_heredoc(commands, vars);
	i = 0;
	while (i < count)
	{
		if (redirs[i].type == '<')
			setup_input_redirection(commands, vars, j);
		else if (redirs[i].type == '>')
			setup_output_redirection(commands, vars);
		if (vars->redirection_failed)
			return ((void)free(redirs));
		i++;
	}
	free(redirs);
	if (!setup_pipe(vars->pipe_fd))
	{
		g_exit_status = 1;
		exit(g_exit_status);
	}
}
