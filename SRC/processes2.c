/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:52:18 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 16:33:30 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_unquoted_char(char *str, char c)
{
	int		in_quotes;
	char	quote_char;

	in_quotes = 0;
	quote_char = '\0';
	while (*str)
	{
		if (*str == '\'' || *str == '"')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = *str;
			}
			else if (*str == quote_char)
			{
				in_quotes = 0;
				quote_char = '\0';
			}
		}
		else if (!in_quotes && *str == c)
			return (str);
		str++;
	}
	return (NULL);
}

static void	update_quote_state(char c, int *in_quotes, char *cur_quote)
{
	if (c == '\'' || c == '"')
	{
		if (!*in_quotes)
		{
			*in_quotes = 1;
			*cur_quote = c;
		}
		else if (c == *cur_quote)
		{
			*in_quotes = 0;
			*cur_quote = '\0';
		}
	}
}

char	*extract_filename_adjacent(char *start, int *len)
{
	int		i;
	int		in_quotes;
	char	cur_quote;

	i = 0;
	in_quotes = 0;
	cur_quote = '\0';
	while (start[i])
	{
		update_quote_state(start[i], &in_quotes, &cur_quote);
		if (!in_quotes && (start[i] == ' ' || start[i] == '\t'
				|| start[i] == '<' || start[i] == '>'))
			break ;
		i++;
	}
	*len = i;
	return (ft_strndup(start, i));
}

static char	*parse_infile_name(char *temp, int *i)
{
	int		in_quotes;
	char	cur_quote;

	*i = 0;
	in_quotes = 0;
	cur_quote = '\0';
	while (temp[*i] && ((temp[*i] != ' ' && temp[*i] != '\t' && temp[*i] != '<'
				&& temp[*i] != '>') || in_quotes))
	{
		if (temp[*i] == '\'' || temp[*i] == '"')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				cur_quote = temp[*i];
			}
			else if (temp[*i] == cur_quote)
			{
				in_quotes = 0;
				cur_quote = '\0';
			}
		}
		(*i)++;
	}
	return (ft_strndup(temp, *i));
}

static int	open_and_assign_fd(t_vars *vars, char *infile)
{
	char	*cleaned_filename;
	int		fd;

	cleaned_filename = remove_quotes_from_string(ft_strdup(infile));
	fd = open(cleaned_filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cleaned_filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_exit_status = 1;
		vars->redirection_failed = true;
		free(cleaned_filename);
		return (-1);
	}
	vars->fd0 = fd;
	vars->infile_name = cleaned_filename;
	return (0);
}

static int	handle_redirection_error(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
		2);
	g_exit_status = 2;
	return (0);
}

static char	*skip_whitespace(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static int	handle_single_redirection(t_vars *vars, char *temp, int *i,
		char **infile)
{
	if (*infile)
		free(*infile);
	*infile = parse_infile_name(temp, i);
	if (!*infile || (*infile)[0] == '\0')
	{
		vars->redirection_failed = true;
		free(*infile);
		return (handle_redirection_error());
	}
	if (vars->fd0 > 0)
		close(vars->fd0);
	if (open_and_assign_fd(vars, *infile) == -1)
	{
		free(*infile);
		return (0);
	}
	return (1);
}

static int	process_infile_token(t_vars *vars, char **temp, int *j, int *i,
		char **infile)
{
	if (*(*temp + 1) == '<')
	{
		handle_heredoc(vars, *temp, j);
		if (vars->redirection_failed)
			return (0);
		*temp += 2;
		return (2);
	}
	(*temp)++;
	*temp = skip_whitespace(*temp);
	if (**temp == '\0')
	{
		vars->redirection_failed = true;
		return (handle_redirection_error());
	}
	if (!handle_single_redirection(vars, *temp, i, infile))
		return (0);
	*temp += *i;
	return (1);
}

int	setup_input_redirection(char **commands, t_vars *vars, int *j)
{
	char	*infile;
	char	*temp;
	int		i;
	int		result;

	temp = commands[0];
	vars->fd0 = 0;
	infile = NULL;
	temp = find_unquoted_char(temp, '<');
	while (temp)
	{
		result = process_infile_token(vars, &temp, j, &i, &infile);
		if (result == 0)
			return (0);
		if (result == 2)
			continue ;
		temp = find_unquoted_char(temp, '<');
	}
	if (infile)
		free(infile);
	return (vars->fd0 > 0);
}

void	handle_output_redirection(t_vars *vars, char *outfile)
{
	vars->outfile_name = remove_quotes_from_string(ft_strdup(outfile));
	free(outfile);
	if (vars->fd1 < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(vars->outfile_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_exit_status = 1;
		vars->redirection_failed = true;
	}
}
