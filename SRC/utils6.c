/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:59 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 15:14:36 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Advanced tokenization helper that skips over complete redirection
// tokens (including their filenames) while respecting quotes.
// Used when parsing commands to ignore redirections.

char	*skip_redirection_token(char *str, char *delimiters)
{
	char	d;

	d = '+';
	while (*str && ft_strchr(delimiters, *str))
		str++;
	while (*str && (!ft_strchr(delimiters, *str) || d != '+'))
	{
		if (*str == '"' || *str == '\'')
		{
			if (*str == d)
				d = '+';
			else if (d == '+')
				d = *str;
		}
		str++;
	}
	return (str);
}

void	handler_quit_ctrlc(int sig)
{
	if (sig == SIGINT || sig == SIGQUIT)
		return ;
}

char	*ft_strjoin_three_help(char *s1, char *s2, char *s3, char *str)
{
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3) + 1;
	str = malloc(sizeof(char) * len);
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, len);
	ft_strlcat(str, s2, len);
	ft_strlcat(str, s3, len);
	return (str);
}

void	first_process_helper(t_vars *vars)
{
	if (vars->fd1 != 1)
		close(vars->fd1);
	if (vars->fd0 != 0)
		close(vars->fd0);
	close(vars->pipe_fd[1]);
	if (vars->p0 != 0)
		close(vars->p0);
}
