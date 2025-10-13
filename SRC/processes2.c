/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:52:18 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 14:33:25 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// When a quote character is encountered:
//  * If not in quotes, enters quote mode with that character
//  * If in quotes and matches current quote, exits quote mode
//  * If in quotes but different quote, treats as regular character

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

// Extracts a filename that immediately follows a redirection operator.
// Stops at whitespace or another redirection when not in quotes.
// Respects quote boundaries.

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

// Parses the filename for input redirection, handling quotes.
// Stops at whitespace or redirection operators when not quoted.

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

// Removes quotes from filename, attempts to open in read-only mode.
// On success, assigns to vars->fd0 and stores cleaned filename.
// On failure, prints error and sets redirection_failed flag.

int	open_and_assign_fd(t_vars *vars, char *infile)
{
	char	*cleaned_filename;
	int		fd;

	cleaned_filename = remove_quotes_from_string(ft_strdup(infile));
	fd = open(cleaned_filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cleaned_filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_exit_status = 1;
		vars->redirection_failed = true;
		free(cleaned_filename);
		return (-1);
	}
	vars->fd0 = fd;
	vars->infile_name = cleaned_filename;
	return (0);
}

// Removes quotes from outfile and stores cleaned name.
// If fd1 indicates an error (< 0), prints error message
// and sets redirection_failed flag.

void	handle_output_redirection(t_vars *vars, char *outfile)
{
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
