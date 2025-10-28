/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:45:27 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/28 19:24:42 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	disable_quit_echo(struct termios *old_term)
{
	struct termios	new_term;

	if (tcgetattr(STDIN_FILENO, old_term) == -1)
		return;
	new_term = *old_term;
	new_term.c_lflag &= ~0001000;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void	restore_terminal(struct termios *old_term)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
		close(STDIN_FILENO);
		get_next_line(-1);
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
