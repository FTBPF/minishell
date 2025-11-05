/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:23:09 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:33:41 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_free_fd_array(int *fds)
{
	int	i;

	if (!fds)
		return ;
	i = 0;
	while (fds[i] != -1)
	{
		if (fds[i] > 0)
			close(fds[i]);
		i++;
	}
	free(fds);
}

static void	ft_free_simple_pointers(t_vars *vars)
{
	if (vars->cmd_flags)
		ft_free(vars->cmd_flags);
	if (vars->temp)
		free(vars->temp);
	if (vars->infile_name)
		free(vars->infile_name);
	if (vars->outfile_name)
		free(vars->outfile_name);
	if (vars->cmd1_path)
		free(vars->cmd1_path);
	vars->cmd_flags = NULL;
	vars->temp = NULL;
	vars->infile_name = NULL;
	vars->outfile_name = NULL;
	vars->cmd1_path = NULL;
}

void	ft_free_vars_in_child(t_vars *vars)
{
	if (!vars)
		return ;
	if (vars->here_doc_fd)
	{
		ft_free_fd_array(vars->here_doc_fd);
		vars->here_doc_fd = NULL;
	}
	ft_free_simple_pointers(vars);
}

int	*exit_status(void)
{
	static int	exit_status;

	return (&exit_status);
}
