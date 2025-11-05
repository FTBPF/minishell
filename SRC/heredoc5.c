/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marada <marada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 11:37:46 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 17:45:07 by marada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	disable_quit_echo(struct termios *old_term)
{
	struct termios	new_term;

	if (tcgetattr(STDIN_FILENO, old_term) == -1)
	{
		printf("hello\n");
		return ;
	}
	new_term = *old_term;
	new_term.c_cc[VQUIT] = _POSIX_VDISABLE;
	new_term.c_lflag &= ~0001000;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void	restore_terminal(struct termios *old_term)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}

void	print_heredoc_error(void)
{
	ft_putstr_fd("minishell: syntax error near", 2);
	ft_putstr_fd(" unexpected token `newline'\n", 2);
	*exit_status() = 2;
}

void	handle_redirection_failure(t_vars *vars, int *printed)
{
	if (vars->redirection_failed && *printed == 0)
	{
		(*printed)++;
		if (vars->temp)
		{
			unlink(vars->temp);
			free(vars->temp);
			vars->temp = NULL;
		}
		if (*exit_status() != 130)
			print_heredoc_error();
	}
}
