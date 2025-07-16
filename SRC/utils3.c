/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marada <marada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:44 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/16 16:26:46 by marada           ###   ########.fr       */
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

// Receives the signal and handles it if it is CTRL+C
// RL_on_new_line() moves the cursor to the next line
// RL_redisplays() redisplays the prompt "myshell> "

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
	vars->fd0 = 0;
	vars->fd1 = 1;
	vars->p0 = 0;
	vars->pipe_fd[0] = -1;
	vars->pipe_fd[1] = -1;
	vars->num_env_vars = 0;
	vars->here_doc_fd = NULL;
	vars->my_environ = NULL;
	vars->cmd2_flags = NULL;
	vars->cmd2_path = NULL;
	vars->cmd_flags = NULL;
	vars->cmd1_path = NULL;
	vars->temp = NULL;
}

int	setup_pipe(int	*pipe_fd)
{
	if (pipe(pipe_fd) < 0)
	{
		perror("Pipe");
		return (0);
	}
	return (1);
}
