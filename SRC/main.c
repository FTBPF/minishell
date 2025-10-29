/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:20 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/28 18:01:03 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			g_exit_status = 0;

void minishell(char *input, char **env, t_vars *vars)
{
    int i;

    vars->redirection_failed = false;
    i = minishell_helper(input, env, vars);  // No commands param
    if (i == 0)
        return;
    close(vars->pipe_fd[0]);
    ft_free_vars(vars);
}

void	setup_shell(t_vars *vars, char **env)
{
	rl_catch_signals = 0;
	rl_set_signals();
	ft_vars_init(vars);
	copy_environ(env, vars);
}

static void	setup_signals_parent(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void run_shell(t_vars *vars, char **env)
{
    char *input;

    while (1)
    {
        setup_signals_parent();
        input = readline("myshell> ");
        if (!ft_exit_ctrl_d(input))
        {
            g_exit_status = 1;
            ft_free(vars->my_environ);
            ft_free_vars(vars);
            rl_clear_history();
            break;
        }
        if (ft_strlen(input) != 0)
        {
            add_history(input);
            minishell(input, env, vars);
			free(input);
        }
        else
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
	return (g_exit_status);
}
