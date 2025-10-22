/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:59 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/22 12:22:38 by frteixei         ###   ########.fr       */
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

// Calculates total length and allocates memory for joined string.
// Copies all three strings sequentially using ft_strlcpy and ft_strlcat.

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
