/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 17:24:01 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/29 11:39:53 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	execute_commands(t_vars *vars, char **env, char **commands)
{
	vars->i = 0;
	vars->p0 = 0;
	vars->j = 0;
	while (commands[vars->i])
	{
		first_process(vars, env, &commands[vars->i], &vars->j);
		free(commands[vars->i]);
		(vars->i)++;
	}
}

static int	collect_status(void)
{
	int	status;
	int	last;
	int	sig;

	last = 0;
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
				ft_putstr_fd("Quit (core dumped)\n", 2);
			else if (sig == SIGINT)
				ft_putstr_fd("\n", 1);
			last = 128 + sig;
		}
	}
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	return (last);
}

int	minishell_helper(char *input, char **env, t_vars *vars, char **commands)
{
	if (!setup_commands(input, vars, &commands))
		return (0);
	execute_commands(vars, env, commands);
	g_exit_status = collect_status();
	if (vars->p0 != 0)
		close(vars->p0);
	free(commands);
	return (vars->i);
}
