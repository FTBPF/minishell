/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:40 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 15:08:24 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Similar to get_token_length, but used in context where redirections
// are handled separately. Respects quotes and stops at delimiters.

int	get_token_length_no_redirection(char *token_start, char *delimiters)
{
	int		length;
	char	current_quote;

	length = 0;
	current_quote = '\0';
	ft_get_token_l_noredirection_helper(&length, token_start, delimiters,
		current_quote);
	return (length);
}

// Advances past delimiters and redirection sequences using funcao_nova.
// Tracks quote state to handle quoted content properly.

char	*get_next_token_no_redirection(char *str, char *delimiters)
{
	int		in_quotes;
	char	current_quote;

	in_quotes = 0;
	current_quote = '\0';
	while (*str)
	{
		if (in_quotes == 0 && ft_strchr(delimiters, *str))
		{
			if (*str == '>' || *str == '<')
				str = skip_redirection_token(str, delimiters);
			else
				str++;
		}
		else
			break ;
		ft_get_next_token_noredirection_helper(*str, &in_quotes,
			&current_quote);
	}
	if (*str != '\0')
		return (str);
	else
		return (NULL);
}

// Counts tokens while treating redirections and their arguments
// as non-tokens. Used to extract just the command and its arguments.

int	count_words_no_redirection(char *str, char *delimiters)
{
	int		count;
	int		token_length;
	char	*token_start;

	count = 0;
	token_start = get_next_token_no_redirection(str, delimiters);
	while (token_start)
	{
		count++;
		token_length = get_token_length_no_redirection(token_start, delimiters);
		token_start = get_next_token_no_redirection(token_start + token_length,
				delimiters);
	}
	return (count);
}

// Gets next token, validates it's not a redirection operator,
// extracts and duplicates it, and stores in tokens array.

void	process_token(char **tokens, char **token_start, char *delimiters,
		int *i)
{
	int	token_length;

	*token_start = get_next_token_no_redirection(*token_start, delimiters);
	if (*token_start && **token_start != '>' && **token_start != '<')
	{
		token_length = get_token_length(*token_start, delimiters);
		tokens[*i] = ft_strndup(*token_start, token_length);
		*token_start += token_length;
		(*i)++;
	}
}

// Splits the command into tokens but excludes redirection operators
// and their arguments. Returns only the actual command arguments.
// Used to prepare cmd_flags for execution.

char	**ft_split_commands_no_redirection(char *str, char *delimiters)
{
	char	**tokens;
	int		num_words;
	char	*token_start;
	int		token_length;
	int		i;

	if (str == NULL)
		return (NULL);
	num_words = count_words_no_redirection(str, delimiters);
	tokens = malloc((num_words + 1) * sizeof(char *));
	if (!tokens)
		return (NULL);
	token_start = (char *)get_next_token_no_redirection(str, delimiters);
	i = 0;
	while (token_start)
	{
		token_length = get_token_length_no_redirection(token_start, delimiters);
		tokens[i++] = ft_strndup(token_start, token_length);
		token_start += token_length;
		token_start = get_next_token_no_redirection(token_start, delimiters);
	}
	tokens[i] = NULL;
	i = 0;
	return (tokens);
}
