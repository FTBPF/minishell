/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:43:18 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:44:17 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_final_fd(t_vars *vars, int *j)
{
	if (vars->here_doc_fd[*j] == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(vars->temp);
		vars->redirection_failed = true;
	}
}

void	handle_fork_error(t_vars *vars, int fd, char *doc_file)
{
	perror("fork");
	close(fd);
	free(doc_file);
	vars->redirection_failed = true;
}
