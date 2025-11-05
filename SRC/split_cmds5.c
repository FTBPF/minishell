/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:21:39 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 15:02:34 by frteixei         ###   ########.fr       */
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

static void	print_eof_warning(char *delimiter, int line_count)
{
	char	*trimmed;
	
	trimmed = ft_strtrim(delimiter, "\n");
	if (!trimmed)
	{
		ft_putstr_fd("minishell: warning: here-document at line ", 2);
		ft_putnbr_fd(line_count, 2);
		ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return;
	}
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(line_count, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(trimmed, 2);
	ft_putstr_fd("')\n", 2);
	free(trimmed);
}

static void	write_line(int fd, char *str, int expand, t_vars *vars)
{
	char	*expanded;

	if (expand)
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
}

void	process_heredoc_expanded(t_vars *vars, char *doc_file, int fd,
		int expand)
{
	char			*str;
	int				lines;
	struct termios	old_term;

	g_heredoc_vars = vars;
	vars->doc_file = doc_file;
	disable_quit_echo(&old_term);
	lines = 1;
	write(1, "> ", 2);
	str = get_next_line(0);
	while (str && ft_strcmp(str, doc_file) != 0)
	{
		write_line(fd, str, expand, vars);
		free(str);
		lines++;
		write(1, "> ", 2);
		str = get_next_line(0);
	}
	if (!str)
		print_eof_warning(doc_file, lines);
	restore_terminal(&old_term);
	free(str);
	get_next_line(-1);
	free(doc_file);
	vars->doc_file = NULL;
	g_heredoc_vars = NULL;
	ft_cleanup_heredoc_child(vars);
	exit(*exit_status() = 0);
}
