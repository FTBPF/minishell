/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:20:08 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 16:20:53 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	get_first_redir_positions(t_redir *redirs, int count,
		int *first_output, int *first_input)
{
	int	i;

	*first_output = -1;
	*first_input = -1;
	i = 0;
	while (i < count)
	{
		if (redirs[i].type == '>' && *first_output == -1)
			*first_output = redirs[i].index;
		if (redirs[i].type == '<' && *first_input == -1)
			*first_input = redirs[i].index;
		i++;
	}
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
	get_first_redir_positions(redirs, count, &first_output, &first_input);
	result = false;
	if (first_output != -1 && first_input != -1 && first_output < first_input)
		result = true;
	free(redirs);
	return (result);
}

static char	*skip_whitespace(char *str)
{
	while (*str == ' ' || *str == '\t')
		str++;
	return (str);
}

static char	*parse_outfile_token(char *temp, int *i)
{
	int		in_quotes;
	char	quote_char;

	*i = 0;
	in_quotes = 0;
	quote_char = '\0';
	while (temp[*i] && ((temp[*i] != ' ' && temp[*i] != '\t' && temp[*i] != '<'
				&& temp[*i] != '>') || in_quotes))
	{
		if (temp[*i] == '\'' || temp[*i] == '"')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				quote_char = temp[*i];
			}
			else if (temp[*i] == quote_char)
			{
				in_quotes = 0;
				quote_char = '\0';
			}
		}
		(*i)++;
	}
	return (remove_quotes_from_string(ft_strndup(temp, *i)));
}

static int	open_input_file(t_vars *vars, char *infile)
{
	if (vars->fd0 > 0)
		close(vars->fd0);
	if (open_and_assign_fd(vars, infile) == -1)
		return (free(infile), 0);
	free(infile);
	return (1);
}

static char	*get_input_filename(char *temp, t_vars *vars, int *i)
{
	char	*infile;

	temp = skip_whitespace(temp);
	if (*temp == '\0')
	{
		vars->redirection_failed = true;
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		g_exit_status = 2;
		return (NULL);
	}
	infile = parse_infile_name(temp, i);
	if (!infile || infile[0] == '\0')
	{
		vars->redirection_failed = true;
		free(infile);
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		g_exit_status = 2;
		return (NULL);
	}
	return (infile);
}

static int	process_single_input_redir(char *cmd, int redir_pos, t_vars *vars, int *j)
{
	char	*temp;
	char	*infile;
	int		i;

	temp = cmd + redir_pos;
	if (*(temp + 1) == '<')
	{
		handle_heredoc(vars, temp, j);
		if (vars->redirection_failed)
			return (0);
		return (1);
	}
	temp++;
	infile = get_input_filename(temp, vars, &i);
	if (!infile)
		return (0);
	return (open_input_file(vars, infile));
}

static int	open_output_file(t_vars *vars, char *outfile, bool is_append)
{
	int	fd;

	if (vars->fd1 > 1)
		close(vars->fd1);
	if (is_append)
		fd = open(outfile, O_CREAT | O_RDWR | O_APPEND, 0644);
	else
		fd = open(outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(outfile, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_exit_status = 1;
		vars->redirection_failed = true;
		free(outfile);
		return (0);
	}
	vars->fd1 = fd;
	vars->outfile_name = outfile;
	return (1);
}

static char	*get_output_filename(char *temp, t_vars *vars, int *i)
{
	temp = skip_whitespace(temp);
	if (*temp == '\0')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
		g_exit_status = 2;
		vars->redirection_failed = true;
		return (NULL);
	}
	return (parse_outfile_token(temp, i));
}

static int	process_single_output_redir(char *cmd, int redir_pos, t_vars *vars)
{
	char	*temp;
	char	*outfile;
	int		i;
	bool	is_append;

	temp = cmd + redir_pos;
	is_append = (*(temp + 1) == '>');
	if (is_append)
		temp += 2;
	else
		temp += 1;
	outfile = get_output_filename(temp, vars, &i);
	if (!outfile)
		return (0);
	return (open_output_file(vars, outfile, is_append));
}

static int	process_redirection(char **commands, t_redir *redirs, 
								int i, t_vars *vars, int *j)
{
	if (redirs[i].type == '<')
	{
		if (!process_single_input_redir(commands[0], redirs[i].index, vars, j))
			return (0);
	}
	else if (redirs[i].type == '>')
	{
		if (!process_single_output_redir(commands[0], redirs[i].index, vars))
			return (0);
	}
	return (1);
}

void	setup_redirections(char **commands, t_vars *vars, int *j)
{
	int		count;
	int		i;
	t_redir	*redirs;

	vars->redirection_failed = false;
	vars->fd0 = 0;
	vars->fd1 = 1;
	count = 0;
	redirs = find_redirections(commands[0], &count);
	i = 0;
	while (i < count && !vars->redirection_failed)
	{
		if (!process_redirection(commands, redirs, i, vars, j))
			return ((void)free(redirs));
		i++;
	}
	free(redirs);
	if (vars->redirection_failed)
		return ;
	if (!setup_pipe(vars->pipe_fd))
	{
		g_exit_status = 1;
		exit(g_exit_status);
	}
}
