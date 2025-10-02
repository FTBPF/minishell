/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:07:07 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/02 13:53:39 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*find_next_output_redirect(char *str)
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
		else if (!in_quotes && *str == '>')
			return (str);
		str++;
	}
	return (NULL);
}

static int	open_outfile(t_vars *vars, char *outfile, bool is_append)
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
		return (-1);
	}
	vars->fd1 = fd;
	vars->outfile_name = outfile;
	return (0);
}

static int	handle_output_redirect(t_vars *vars, char **temp_ptr, int *last_fd)
{
	char	*outfile;
	int		i;
	bool	is_append;
	char	*temp;

	temp = *temp_ptr;
	is_append = (*(temp + 1) == '>');
	if (is_append)
		temp += 2;
	else
		temp += 1;
	while (*temp == ' ' || *temp == '\t')
		temp++;
	if (*temp == '\0')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putendl_fd("`newline'", 2);
		g_exit_status = 2;
		vars->redirection_failed = true;
		return (0);
	}
	outfile = parse_outfile_token(temp, &i);
	if (open_outfile(vars, outfile, is_append) == -1)
		return (0);
	return (*last_fd = vars->fd1, *temp_ptr = temp + i, 1);
}

int	setup_output_redirection(char **commands, t_vars *vars)
{
	char	*temp;
	int		last_valid_fd;

	last_valid_fd = -1;
	temp = commands[0];
	vars->fd1 = 1;
	vars->redirection_failed = false;
	temp = find_next_output_redirect(temp);
	while (temp)
	{
		if (!handle_output_redirect(vars, &temp, &last_valid_fd))
			return (0);
		temp = find_next_output_redirect(temp);
	}
	if (last_valid_fd != -1)
		return (1);
	return (0);
}
