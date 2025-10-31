/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:22:21 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/31 12:58:29 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	skip_token_with_quotes(const char *temp, int *i)
{
	int		in_quotes;
	char	quote_char;

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
}

char	*parse_outfile_token(char *temp, int *i)
{
	char	*temp_str;
	char	*result;

	*i = 0;
	skip_token_with_quotes(temp, i);
	temp_str = ft_strndup(temp, *i);
	result = remove_quotes_from_string(temp_str);
	free(temp_str);
	return (result);
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
		ft_putstr_fd("minishell: syntax error near unexpected ", 2);
		ft_putstr_fd("token `newline'\n", 2);
		g_exit_status = 2;
		return (NULL);
	}
	infile = parse_infile_name(temp, i);
	if (!infile || infile[0] == '\0')
	{
		vars->redirection_failed = true;
		if (infile)
			free(infile);
		ft_putstr_fd("minishell: syntax error near unexpected ", 2);
		ft_putstr_fd("token `newline'\n", 2);
		g_exit_status = 2;
		return (NULL);
	}
	return (infile);
}

int	process_single_input_redir(char *cmd, int pos, t_vars *vars, int *j)
{
	char	*temp;
	char	*infile;
	int		i;

	temp = cmd + pos;
	if (*temp == '<' && *(temp + 1) == '<')
	{
		if (!vars->here_doc_fd || vars->here_doc_fd[*j] == -1)
			return (vars->redirection_failed = true, 0);
		if (vars->fd0 > 0 && vars->fd0 != STDIN_FILENO)
			close(vars->fd0);
		vars->fd0 = vars->here_doc_fd[*j];
		(*j)++;
		return (1);
	}
	if (*temp == '<')
	{
		temp++;
		infile = get_input_filename(temp, vars, &i);
		if (!infile)
			return (0);
		return (open_input_file(vars, infile));
	}
	return (0);
}
