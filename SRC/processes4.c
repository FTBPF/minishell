/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:47:57 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 14:59:49 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_redirection_error(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
		2);
	g_exit_status = 2;
	return (0);
}

// Extracts the filename, validates it's not empty, closes previous fd0,
// and opens the new input file. Updates *infile with new filename.

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

// Determines if redirection is heredoc (<<) or regular input (<).
// For heredoc, calls handle_heredoc. For regular input, validates
// filename exists and calls handle_single_redirection.

static int	process_infile_token(t_vars *vars, t_redirection_context *ctx)
{
	if (*(*(ctx->temp) + 1) == '<')
	{
		handle_heredoc(vars, *(ctx->temp), ctx->j);
		if (vars->redirection_failed)
			return (0);
		*(ctx->temp) += 2;
		return (2);
	}
	(*(ctx->temp))++;
	*(ctx->temp) = skip_whitespace(*(ctx->temp));
	if (**(ctx->temp) == '\0')
	{
		vars->redirection_failed = true;
		return (handle_redirection_error());
	}
	if (!handle_single_redirection(vars, *(ctx->temp), ctx->i, ctx->infile))
		return (0);
	*(ctx->temp) += *(ctx->i);
	return (1);
}

// Finds all input redirections (< and <<) in the command and
// processes them in order. Only the last redirection takes effect.
// Uses a context structure to pass multiple values to helper function.

int	setup_input_redirection(char **commands, t_vars *vars, int *j)
{
	char					*infile;
	char					*temp;
	int						i;
	int						result;
	t_redirection_context	ctx;

	infile = NULL;
	temp = commands[0];
	vars->fd0 = 0;
	temp = find_unquoted_char(temp, '<');
	ctx.temp = &temp;
	ctx.i = &i;
	ctx.j = j;
	ctx.infile = &infile;
	while (temp)
	{
		result = process_infile_token(vars, &ctx);
		if (result == 0)
			return (0);
		if (result != 2)
			temp = find_unquoted_char(temp, '<');
	}
	if (infile)
		free(infile);
	return (vars->fd0 > 0);
}
