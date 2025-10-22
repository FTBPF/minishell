/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:27 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/22 12:16:13 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Splits str into tokens separated by any character in delimiters,
// respecting quotes. Allocates and returns an array of strings.
// Handles memory allocation errors by freeing partial results.

char	**ft_split_commands(char *str, char *delimiters)
{
	int		num_words;
	char	**words;
	char	*token_start;
	int		token_length;
	int		i;

	num_words = count_words(str, delimiters);
	words = (char **)malloc((num_words + 1) * sizeof(char *));
	if (!words)
		return (NULL);
	i = 0;
	token_start = get_next_token(str, delimiters);
	while (token_start)
	{
		token_length = get_token_length(token_start, delimiters);
		words[i] = ft_strndup(token_start, token_length);
		if (!words[i++])
		{
			ft_free(words);
			return (NULL);
		}
		token_start = get_next_token(token_start + token_length, delimiters);
	}
	words[num_words] = NULL;
	return (words);
}

// Searches for "<<" that is not inside single or double quotes.
// Tracks quote state while iterating through the string.

int	has_unquoted_heredoc(const char *s)
{
	int	in_squotes;
	int	in_dquotes;

	in_squotes = 0;
	in_dquotes = 0;
	while (*s)
	{
		if (*s == '\'' && !in_dquotes)
			in_squotes = !in_squotes;
		else if (*s == '"' && !in_squotes)
			in_dquotes = !in_dquotes;
		else if (*s == '<' && *(s + 1) == '<' && !in_squotes && !in_dquotes)
			return (1);
		s++;
	}
	return (0);
}

static bool	is_escaped(const char *str, int pos)
{
	int	backslashes;

	backslashes = 0;
	pos--;
	while (pos >= 0 && str[pos] == '\\')
	{
		backslashes++;
		pos--;
	}
	return ((backslashes % 2) == 1);
}

// Iterates through str, tracking quote state and recording positions
// and types of '<' and '>' characters found outside quotes and not escaped.
// Updates *count with the number of redirections found.

static void	scan_redirections(const char *str, t_redir *results, int *count)
{
	bool	in_single;
	bool	in_double;
	int		i;
	char	c;

	in_single = false;
	in_double = false;
	i = 0;
	while (str[i] != '\0')
	{
		c = str[i];
		if (c == '\'' && !in_double && !is_escaped(str, i))
			in_single = !in_single;
		else if (c == '"' && !in_single && !is_escaped(str, i))
			in_double = !in_double;
		if (!in_single && !in_double && (c == '<' || c == '>'))
		{
			if (i == 0 || str[i - 1] != c)
			{
				results[*count].type = c;
				results[*count].index = i;
				(*count)++;
			}
		}
		i++;
	}
}

// Allocates an array to store redirection information, calls
// scan_redirections to find them, and reallocates if necessary.
// Sets *count to the number of redirections found.

t_redir	*find_redirections(const char *str, int *count)
{
	int		capacity;
	t_redir	*results;

	capacity = 16;
	*count = 0;
	results = malloc(capacity * sizeof(t_redir));
	if (!results)
		return (NULL);
	scan_redirections(str, results, count);
	if (*count >= capacity)
	{
		capacity *= 2;
		results = realloc(results, capacity * sizeof(t_redir));
		if (!results)
		{
			perror("realloc");
			exit(1);
		}
	}
	return (results);
}
