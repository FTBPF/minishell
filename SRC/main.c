/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:20 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:38:24 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	setup_shell(t_vars *vars, char **env)
{
	char	*shlvl_str;
	char	*new_shlvl_str;
	int		shlvl;

	rl_catch_signals = 0;
	rl_set_signals();
	ft_vars_init(vars);
	copy_environ(env, vars);
	shlvl_str = get_env_var(vars, "SHLVL");
	if (shlvl_str)
		shlvl = ft_atoi(shlvl_str) + 1;
	else
		shlvl = 1;
	vars->shelllevel = shlvl;
	new_shlvl_str = ft_itoa(shlvl);
	modify_env_var(vars, "SHLVL", new_shlvl_str);
	free(new_shlvl_str);
}

static void	setup_signals_parent(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	handle_ctrl_d(t_vars *vars)
{
	*exit_status() = 1;
	ft_free(vars->my_environ);
	vars->my_environ = NULL;
	ft_free_vars(vars);
	rl_clear_history();
}

void	run_shell(t_vars *vars, char **env)
{
	char	*input;

	while (1)
	{
		setup_signals_parent();
		input = readline("myshell> ");
		if (!ft_exit_ctrl_d(input))
		{
			handle_ctrl_d(vars);
			break ;
		}
		if (ft_strlen(input))
		{
			add_history(input);
			minishell(input, env, vars);
			if (vars)
				ft_free_vars(vars);
		}
		free(input);
	}
}

int	main(int ac, char **av, char **env)
{
	t_vars	vars;

	(void)ac;
	(void)av;
	setup_shell(&vars, env);
	run_shell(&vars, env);
	return (*exit_status());
}
