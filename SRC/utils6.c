/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:23:00 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/22 13:23:02 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	handler_quit(int signal)
{
	if (signal == SIGQUIT)
		write(2, "Quit (core dumped)\n", 20);
	return ;
}

int	ft_is_empty_command(const char *cmd)
{
	char	*trimmed;

	if (!cmd)
		return (1);
	trimmed = skip_whitespace((char *)cmd);
	while (*trimmed)
	{
		if (*trimmed != '|')
			return (0);
		trimmed++;
	}
	return (1);
}
