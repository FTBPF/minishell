/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:18:00 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 13:18:48 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler_quit(int signal)
{
	if (signal == SIGQUIT)
		write(2, "Quit (core dumped)\n", 20);
	return ;
}
