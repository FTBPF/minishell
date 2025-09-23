/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:12 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/23 14:59:25 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int			g_exit_status = 0;

// returns i so that the processes dont interrupt each other (while loop)
int	minishell_helper(char *input, char **env, t_vars *vars, char **commands)
{
	int	status;
	int	last_status;

	last_status = 0;
	if (str_is_spaces_only(input))
		return (0);
	commands = ft_split_commands(input, "|");
	if (!commands)
		return (0);
	vars->in_pipeline = (commands[1] != NULL);
	if (ft_strchr(input, '$'))
		var_expander(vars, commands);
	if (check_cd_ex_uns(commands, vars))
	{
		ft_free_vars(vars);
		ft_free(commands);
		return (0);
	}
	here_doc(vars, commands);
	vars->i = 0;
	vars->p0 = 0;
	vars->j = 0;
	while (commands[vars->i])
	{
		first_process(vars, env, &commands[vars->i], &vars->j);
		free(commands[vars->i]);
		(vars->i)++;
	}
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 0;
	}
	g_exit_status = last_status;
	if (vars->p0 != 0)
		close(vars->p0);
	free(commands);
	return (vars->i);
}

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

// rl_catch_signals = 0; // Disables the default behavior of SIGINT and SIGQUIT
// rl_set_signals(); // Tells readline to ignore the default behaviour of those
// signals and respect the ones we set in signal_handler()
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
