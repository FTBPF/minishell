/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:45:27 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 17:08:31 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_vars	*g_heredoc_vars = NULL;

char	*ft_strndup_aspas(char *commands, int len)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = (char *)malloc(len + 1);
	if (!new_str && !commands)
		return (NULL);
	ft_aspas_helper(len, &i, new_str, commands);
	new_str[i] = '\0';
	return (new_str);
}

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		*exit_status() = 130;
		write(1, "^C\n", 3);
		close(STDIN_FILENO);
		get_next_line(-1);
		if (g_heredoc_vars)
			ft_cleanup_heredoc_child(g_heredoc_vars);
		exit(130);
	}
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	check_open_doc(t_vars *vars, char *doc_file, char *temp_name)
{
	if (!doc_file)
	{
		free(temp_name);
		return (vars->redirection_failed = true, (void)0);
	}
	if (vars->temp)
	{
		unlink(vars->temp);
		free(vars->temp);
	}
}
