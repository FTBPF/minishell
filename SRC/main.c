/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:12 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/09 16:21:45 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			g_exit_status = 0;

// Processes the input by:
// * Checking for whitespace-only input
// * Splitting into commands
// * Expanding variables
// * Handling built-ins that need parent process
// * Processing heredocs
// * Executing commands via minishell_helper
// Cleans up resources before returning.

void	minishell(char *input, char **env, t_vars *vars, char **commands)
{
	int	i;

	i = 0;
	commands = NULL;
	i = minishell_helper(input, env, vars, commands);
	if (i == 0)
		return ;
	close(vars->pipe_fd[0]);
	if (commands)
		ft_free(commands);
	if (input)
		free(input);
	ft_free_vars(vars);
	input = NULL;
}

// Sets up signal handling by disabling readline's default signals
// and using custom handlers. Initializes vars structure and copies
// environment variables.

void	setup_shell(t_vars *vars, char **env)
{
	rl_catch_signals = 0;
	rl_set_signals();
	ft_vars_init(vars);
	copy_environ(env, vars);
}

// Configures signal handling:
// * SIGINT (Ctrl+C): handled by signal_handler
// * SIGQUIT (Ctrl+\): ignored

static void	setup_signals_parent(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

// Runs the interactive shell loop:
// * Sets up signals
// * Reads input with readline
// * Handles Ctrl+D (EOF)
// * Adds non-empty input to history
// * Processes commands
// Continues until Ctrl+D or exit. Cleans up on exit.

void	run_shell(t_vars *vars, char **env)
{
	char	*input;
	char	**commands;

	commands = NULL;
	while (1)
	{
		setup_signals_parent();
		input = readline("myshell> ");
		if (!ft_exit_ctrl_d(input))
		{
			g_exit_status = 1;
			if (commands)
				ft_free(commands);
			ft_free(vars->my_environ);
			ft_free_vars(vars);
			break ;
		}
		if (ft_strlen(input) != 0)
			add_history(input);
		if (ft_strlen(input) != 0)
			minishell(input, env, vars, commands);
	}
}

int	main(int ac, char **av, char **env)
{
	t_vars	vars;

	(void)ac;
	(void)av;
	setup_shell(&vars, env);
	run_shell(&vars, env);
	return (g_exit_status);
}
