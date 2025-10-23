/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 16:47:42 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/23 18:11:41 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_quote_state(char c, int *in_quotes, char *cur_quote)
{
	if (c == '\'' || c == '"')
	{
		if (!*in_quotes)
		{
			*in_quotes = 1;
			*cur_quote = c;
		}
		else if (c == *cur_quote)
		{
			*in_quotes = 0;
			*cur_quote = '\0';
		}
	}
}

char	*extract_filename_adjacent(char *start, int *len)
{
	int		i;
	int		in_quotes;
	char	cur_quote;

	i = 0;
	in_quotes = 0;
	cur_quote = '\0';
	while (start[i])
	{
		update_quote_state(start[i], &in_quotes, &cur_quote);
		if (!in_quotes && (start[i] == ' ' || start[i] == '\t'
				|| start[i] == '<' || start[i] == '>'))
			break ;
		i++;
	}
	*len = i;
	return (ft_strndup(start, i));
}

char	*parse_infile_name(char *temp, int *i)
{
	int		in_quotes;
	char	cur_quote;

	*i = 0;
	in_quotes = 0;
	cur_quote = '\0';
	while (temp[*i] && ((temp[*i] != ' ' && temp[*i] != '\t' && temp[*i] != '<'
				&& temp[*i] != '>') || in_quotes))
	{
		if (temp[*i] == '\'' || temp[*i] == '"')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				cur_quote = temp[*i];
			}
			else if (temp[*i] == cur_quote)
			{
				in_quotes = 0;
				cur_quote = '\0';
			}
		}
		(*i)++;
	}
	return (ft_strndup(temp, *i));
}

int	open_and_assign_fd(t_vars *vars, char *infile)
{
	char	*cleaned_filename;
	int		fd;

	cleaned_filename = remove_quotes_from_string(infile);
	if (!cleaned_filename)
	{
		g_exit_status = 1;
		return (vars->redirection_failed = true, -1);
	}
	fd = open(cleaned_filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cleaned_filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_exit_status = 1;
		vars->redirection_failed = true;
		return (free(cleaned_filename), -1);
	}
	vars->fd0 = fd;
	if (vars->infile_name)
		free(vars->infile_name);
	vars->infile_name = cleaned_filename;
	return (0);
}

void	handle_output_redirection(t_vars *vars, char *outfile)
{
	if (vars->outfile_name)
		free(vars->outfile_name);
	vars->outfile_name = remove_quotes_from_string(ft_strdup(outfile));
	free(outfile);
	if (vars->fd1 < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(vars->outfile_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_exit_status = 1;
		vars->redirection_failed = true;
	}
}
