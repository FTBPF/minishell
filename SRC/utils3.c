/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:44 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/23 17:15:03 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_free_vars_helper(t_vars *vars)
{
	if (vars->infile_name)
	{
		free(vars->infile_name);
		vars->infile_name = NULL;
	}
	if (vars->outfile_name)
	{
		free(vars->outfile_name);
		vars->outfile_name = NULL;
	}
	if (vars->temp)
	{
		unlink(vars->temp);
		free(vars->temp);
		vars->temp = NULL;
	}
}

void	ft_free_vars(t_vars *vars)
{
	int	i;

	if (vars->cmd_flags)
	{
		ft_free(vars->cmd_flags);
		vars->cmd_flags = NULL;
	}
	if (vars->here_doc_fd)
	{
		i = 0;
		while (vars->here_doc_fd[i] != -1)
		{
			if (vars->here_doc_fd[i] > 0)
				close(vars->here_doc_fd[i]);
			i++;
		}
		free(vars->here_doc_fd);
		vars->here_doc_fd = NULL;
	}
	ft_free_vars_helper(vars);
}

void	ft_vars_init(t_vars *vars)
{
	vars->num_env_vars = 0;
	vars->here_doc_fd = NULL;
	vars->my_environ = NULL;
	vars->cmd2_flags = NULL;
	vars->cmd2_path = NULL;
	vars->cmd_flags = NULL;
	vars->cmd1_path = NULL;
	vars->temp = NULL;
	vars->in_pipeline = false;
	vars->infile_name = NULL;
	vars->outfile_name = NULL;
	vars->redirection_failed = false;
}

int	setup_pipe(int *pipe_fd)
{
	if (pipe(pipe_fd) < 0)
	{
		perror("Pipe");
		return (0);
	}
	return (1);
}
