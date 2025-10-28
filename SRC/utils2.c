/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:22:32 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/28 16:25:18 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("^C\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}
