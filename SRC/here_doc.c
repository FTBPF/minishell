/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:57 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/23 17:13:00 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	here_doc(t_vars *vars, char **commands)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (commands[i])
	{
		if (has_unquoted_heredoc(commands[i]))
			j++;
		i++;
	}
	vars->here_doc_fd = malloc(sizeof(char *) * (j + 1));
	if (!vars->here_doc_fd)
		return ;
	vars->here_doc_fd[j] = -1;
	i = 0;
	j = 0;
	while (commands[i])
	{
		tmp = commands[i];
		handle_heredoc(vars, tmp, &j);
		i++;
	}
}

static void	print_heredoc_error(void)
{
	ft_putstr_fd("minishell: syntax error near", 2);
	ft_putstr_fd(" unexpected token `newline'\n", 2);
	g_exit_status = 2;
}

static void	handle_redirection_failure(t_vars *vars, int *printed)
{
	if (vars->redirection_failed && *printed == 0)
	{
		(*printed)++;
		if (vars->temp)
		{
			unlink(vars->temp);
			free(vars->temp);
			vars->temp = NULL;
		}
		print_heredoc_error();
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
