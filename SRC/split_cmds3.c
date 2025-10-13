/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:21:01 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 15:05:17 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Skips past delimiters (respecting quotes) until a non-delimiter
// is found. Tracks quote state to avoid treating quoted delimiters
// as actual delimiters.

char	*get_next_token(char *str, char *delimiters)
{
	int		in_quotes;
	char	current_quote;

	in_quotes = -1;
	current_quote = '\0';
	while (*str)
	{
		if (in_quotes == -1 && !ft_strchr(delimiters, *str))
			break ;
		if ((*str == '\'' || *str == '\"') && (in_quotes == -1
				|| current_quote == *str))
		{
			in_quotes *= -1;
			if (in_quotes == 1)
				current_quote = *str;
			else
				current_quote = '\0';
		}
		str++;
	}
	if (*str)
		return (str);
	else
		return (NULL);
}

// Counts characters until a delimiter is found outside of quotes.
// Respects quote boundaries when determining token end.

int	get_token_length(char *token_start, char *delimiters)
{
	int		length;
	int		in_quotes;
	char	current_quote;

	length = 0;
	in_quotes = -1;
	current_quote = '\0';
	while (*token_start)
	{
		if (in_quotes == -1 && ft_strchr(delimiters, *token_start))
			break ;
		if ((*token_start == '\'' || *token_start == '\"') && (in_quotes == -1
				|| current_quote == *token_start))
		{
			in_quotes *= -1;
			if (in_quotes == 1)
				current_quote = *token_start;
			else
				current_quote = '\0';
		}
		token_start++;
		length++;
	}
	return (length);
}

// Scans through redirections to find the first occurrence of each type.
// Sets pointers to -1 if type not found.

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

// Finds all redirections and checks if the first '>' comes before
// the first '<' in the command. Useful for determining redirection order.

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

char	*skip_whitespace(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}
