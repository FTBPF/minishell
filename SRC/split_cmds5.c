/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:21:39 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/27 17:28:55 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	process_heredoc(t_vars *vars, char *doc_file, int fd)
{
	char	*str;

	write(1, "> ", 2);
	str = get_next_line(0);
	while (str && ft_strcmp(str, doc_file) != 0)
	{
		write(fd, str, ft_strlen(str));
		free(str);
		write(1, "> ", 2);
		str = get_next_line(0);
	}
	free(str);
	str = NULL;
	get_next_line(-1);
	free(doc_file);
	g_exit_status = 0;
	if (vars->my_environ)
		ft_free(vars->my_environ);
	exit(g_exit_status);
}
