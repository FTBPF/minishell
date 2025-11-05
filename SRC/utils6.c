/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:23:00 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:23:28 by frteixei         ###   ########.fr       */
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

static void	ft_free_2d_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	ft_cleanup_heredoc_child(t_vars *vars)
{
	if (!vars)
		return ;
	if (vars->doc_file)
		free(vars->doc_file);
	if (vars->all_commands)
		ft_free_2d_array(vars->all_commands);
	if (vars->here_doc_fd)
		free(vars->here_doc_fd);
	if (vars->temp)
		free(vars->temp);
	if (vars->my_environ)
		ft_free(vars->my_environ);
	vars->doc_file = NULL;
	vars->all_commands = NULL;
	vars->here_doc_fd = NULL;
	vars->temp = NULL;
	vars->my_environ = NULL;
}
