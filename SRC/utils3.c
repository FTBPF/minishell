/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:44 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 15:09:23 by frteixei         ###   ########.fr       */
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

// When SIGINT is received:
//  * Prints "^C" and newline
//  * Clears the current line buffer
//  * Moves to new line
//  * Redisplays the prompt

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("^C\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	return ;
}

void	ft_free_vars(t_vars *vars)
{
	if (vars->cmd_flags)
	{
		ft_free(vars->cmd_flags);
		vars->cmd_flags = NULL;
	}
	if (vars->here_doc_fd)
	{
		free(vars->here_doc_fd);
		vars->here_doc_fd = NULL;
	}
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
