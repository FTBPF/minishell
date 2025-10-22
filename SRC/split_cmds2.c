/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:20:08 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/22 12:16:05 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Closes previous fd1 if open. Opens the file in the appropriate mode
// (append or truncate with create). On success, sets vars->fd1 and
// stores the filename. On failure, prints error and sets redirection_failed.

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

// Skips whitespace, checks for empty filename (syntax error),
// and parses the filename token using parse_outfile_token.

static char	*get_output_filename(char *temp, t_vars *vars, int *i)
{
	temp = skip_whitespace(temp);
	if (*temp == '\0')
	{
		ft_putstr_fd("minishell: syntax error near unexpected ", 2);
		ft_putstr_fd("token `newline'\n", 2);
		g_exit_status = 2;
		vars->redirection_failed = true;
		return (NULL);
	}
	return (parse_outfile_token(temp, i));
}

// Determines redirection type (> or >>), extracts the filename,
// and opens the output file with the appropriate mode.

static int	process_single_output_redir(char *cmd, int redir_pos, t_vars *vars)
{
	char	*temp;
	char	*outfile;
	int		i;
	bool	is_append;

	temp = cmd + redir_pos;
	is_append = false;
	if (*temp == '>' && *(temp + 1) == '>')
	{
		is_append = true;
		temp += 2;
	}
	else if (*temp == '>')
		temp += 1;
	else
		return (0);
	outfile = get_output_filename(temp, vars, &i);
	if (!outfile)
		return (0);
	return (open_output_file(vars, outfile, is_append));
}

// Routes to the appropriate handler based on redirection type:
//  - '<': calls process_single_input_redir
//  - '>': calls process_single_output_redir

static int	process_redirection(char *command, t_redir *redir, t_vars *vars,
		int *j)
{
	if (redir->type == '<')
	{
		if (!process_single_input_redir(command, redir->index, vars, j))
			return (0);
	}
	else if (redir->type == '>')
	{
		if (!process_single_output_redir(command, redir->index, vars))
			return (0);
	}
	return (1);
}

// Main redirection setup function:
//  * Initializes fd0 and fd1
//  * Finds all redirections in the command
//  * Processes each redirection in order
//  * Sets up pipe if needed
// Stops early if redirection_failed is set.

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
	if (!redirs && count > 0)
	{
		vars->redirection_failed = true;
		return ;
	}
	i = 0;
	while (i < count && !vars->redirection_failed)
	{
		if (!process_redirection(commands[0], &redirs[i], vars, j))
		{
			free(redirs);
			return ;
		}
		i++;
	}
	if (redirs)
		free(redirs);
	if (vars->redirection_failed)
		return ;
	if (!setup_pipe(vars->pipe_fd))
	{
		g_exit_status = 1;
		exit(g_exit_status);
	}
}
