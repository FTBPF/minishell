/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:27 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 12:03:19 by frteixei         ###   ########.fr       */
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
/* static void	debug_print_tokens(char **tokens)
{
	int		i;

	i = 0;
	printf("DEBUG: Tokens found:\n");
	while (tokens && tokens[i])
	{
		printf("  [%d]: '%s'\n", i, tokens[i]);
		i++;
	}
	printf("  Total tokens: %d\n", i);
} */

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
	// debug_print_tokens(words);
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

t_redir	*find_redirections(const char *str, int *count)
{
	int		capacity;
	t_redir	*results;
	bool	in_single;
	bool	in_double;
	char	c;

	capacity = 8;
	results = malloc(capacity * sizeof(t_redir));
	*count = 0;
	in_single = false;
	in_double = false;
	for (int i = 0; str[i] != '\0'; i++)
	{
		c = str[i];
		if (c == '\'' && !in_double && !is_escaped(str, i))
			in_single = !in_single;
		else if (c == '"' && !in_single && !is_escaped(str, i))
			in_double = !in_double;
		if (!in_single && !in_double && (c == '<' || c == '>'))
		{
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
			results[*count].type = c;
			results[*count].index = i;
			(*count)++;
		}
	}
	return (results);
}

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
	result = false;
	first_output = -1;
	first_input = -1;
	for (int i = 0; i < count; i++)
	{
		if (redirs[i].type == '>' && first_output == -1)
			first_output = redirs[i].index;
		if (redirs[i].type == '<' && first_input == -1)
			first_input = redirs[i].index;
	}
	if (first_output != -1 && first_input != -1 && first_output < first_input)
		result = true;
	free(redirs);
	return (result);
}

void	setup_redirections(char **commands, t_vars *vars, int *j)
{
	int		count;
	t_redir	*redirs;

	// printf("DEBUG: setup_redirections called with commands[0] = '%s'\n",
	// 		commands[0]);
	vars->redirection_failed = false;
	count = 0;
	redirs = find_redirections(commands[0], &count);
	if (has_unquoted_heredoc(commands[0]))
		here_doc(vars, vars->cmd_flags);
	for (int i = 0; i < count; i++)
	{
		if (redirs[i].type == '<')
			setup_input_redirection(commands, vars, j);
		else if (redirs[i].type == '>')
			setup_output_redirection(commands, vars);
		if (vars->redirection_failed)
		{
			free(redirs);
			return ;
		}
	}
	free(redirs);
	if (!setup_pipe(vars->pipe_fd))
	{
		g_exit_status = 1;
		exit(g_exit_status);
	}
}
