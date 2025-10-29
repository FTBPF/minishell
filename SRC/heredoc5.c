/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 11:37:46 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/29 11:51:26 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	disable_quit_echo(struct termios *old_term)
{
	struct termios	new_term;

	if (tcgetattr(STDIN_FILENO, old_term) == -1)
		return ;
	new_term = *old_term;
	new_term.c_cc[VQUIT] = _POSIX_VDISABLE;
	new_term.c_lflag &= ~0001000;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}

void	restore_terminal(struct termios *old_term)
{
	tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}
