/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:27 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/18 11:10:06 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_next_token(char *str, char *delimiters)
{
	int			in_quotes;
	char		current_quote;

	in_quotes = -1;
	current_quote = '\0';
	while (*str)
	{
		if (in_quotes == -1 && !ft_strchr(delimiters, *str))
			break ;
		if ((*str == '\'' || *str == '\"')
			&& (in_quotes == -1 || current_quote == *str))
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
		if ((*token_start == '\'' || *token_start == '\"')
			&& (in_quotes == -1 || current_quote == *token_start))
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

char	**ft_split_commands(char *str, char *delimiters)
{
	int			num_words;
	char		**words;
	char		*token_start;
	int			token_length;
	int			i;

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

void	setup_redirections(char **commands, t_vars *vars, int *j)
{
	vars->redirection_failed = false;
	if (ft_strrchr(commands[0], '<'))
		setup_input_redirection(commands, vars, j);
	if (ft_strrchr(commands[0], '>'))
		setup_output_redirection(commands, vars);
	if (!setup_pipe(vars->pipe_fd))
	{
		g_exit_status = 1;
		exit(g_exit_status);
	}
}
