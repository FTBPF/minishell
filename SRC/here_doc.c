/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:57 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:36:12 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_here_doc_fds(t_vars *vars, int count)
{
	int	k;

	k = 0;
	while (k <= count)
	{
		vars->here_doc_fd[k] = -1;
		k++;
	}
}

static int	count_heredocs(char **commands)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (commands[i])
	{
		if (has_unquoted_heredoc(commands[i]))
			count++;
		i++;
	}
	return (count);
}

void	here_doc(t_vars *vars, char **commands)
{
	int		i;
	int		j;
	int		count;
	char	*tmp;

	count = count_heredocs(commands);
	vars->here_doc_fd = malloc(sizeof(int) * (count + 1));
	if (!vars->here_doc_fd)
		return (ft_free_vars(vars));
	init_here_doc_fds(vars, count);
	i = -1;
	j = 0;
	while (commands[++i])
	{
		tmp = commands[i];
		handle_heredoc(vars, tmp, &j);
		if (vars->redirection_failed)
			return ;
	}
}

void	handle_heredoc(t_vars *vars, char *tmp, int *j)
{
	char		*after;
	char		*next;
	static int	printed = 0;

	while (has_unquoted_heredoc(tmp))
	{
		after = ft_strchr(tmp, '<');
		if (!after || *(after + 1) != '<')
			break ;
		after += 2;
		while (*after == ' ' || *after == '\t')
			after++;
		open_doc(vars, tmp, j);
		handle_redirection_failure(vars, &printed);
		if (vars->redirection_failed)
			return ;
		next = ft_strchr(after, '<');
		if (next && *(next + 1) == '<')
			if (vars->here_doc_fd && vars->here_doc_fd[*j] > 0)
				close(vars->here_doc_fd[*j]);
		(*j)++;
		tmp = after;
	}
	printed = 0;
}

void	ft_aspas_helper(int len, int *i, char *new_str, char *commands)
{
	char	current_quote;
	int		in_quotes;
	int		j;

	j = 0;
	in_quotes = -1;
	current_quote = '\0';
	while (j < len)
	{
		if ((in_quotes == 1 && current_quote != commands[j])
			|| (commands[j] != '\'' && commands[j] != '\"'))
			new_str[(*i)++] = commands[j];
		if ((commands[j] == '\'' || commands[j] == '\"') && (in_quotes == -1
				|| current_quote == commands[j]))
		{
			in_quotes *= -1;
			if (in_quotes == 1)
				current_quote = commands[j];
			else
				current_quote = '\0';
		}
		j++;
	}
}
