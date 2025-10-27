/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:21:39 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/27 17:39:05 by frteixei         ###   ########.fr       */
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

static char	*expand_heredoc_line(t_vars *vars, char *line)
{
	char	*result;
	char	*temp;
	int		i;
	int		j;

	if (!ft_strchr(line, '$'))
		return (ft_strdup(line));
	
	result = ft_strdup("");
	if (!result)
		return (NULL);
	
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '?')
		{
			temp = ft_itoa(g_exit_status);
			result = ft_strjoin(result, temp);
			free(temp);
			i += 2;
		}
		else if (line[i] == '$' && line[i + 1] && line[i + 1] != ' ' 
			&& line[i + 1] != '\t' && line[i + 1] != '\n')
		{
			j = i + 1;
			while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
				j++;
			
			if (j > i + 1)
			{
				char *var_name = ft_substr(line, i + 1, j - i - 1);
				char *var_value = get_env_var(vars, var_name);
				
				if (var_value)
				{
					temp = result;
					result = ft_strjoin(result, var_value);
					free(temp);
				}
				free(var_name);
				i = j;
			}
			else
			{
				temp = result;
				result = ft_strjoin_char(result, line[i]);
				free(temp);
				i++;
			}
		}
		else
		{
			temp = result;
			result = ft_strjoin_char(result, line[i]);
			free(temp);
			i++;
		}
	}
	
	return (result);
}

void	process_heredoc_expanded(t_vars *vars, char *doc_file, int fd, int should_expand)
{
	char	*str;
	char	*expanded;

	write(1, "> ", 2);
	str = get_next_line(0);
	while (str && ft_strcmp(str, doc_file) != 0)
	{
		if (should_expand)
		{
			expanded = expand_heredoc_line(vars, str);
			if (expanded)
			{
				write(fd, expanded, ft_strlen(expanded));
				free(expanded);
			}
		}
		else
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
