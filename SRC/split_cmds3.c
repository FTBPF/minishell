/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:21:01 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 16:21:26 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
