/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:12 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/08 16:36:25 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// returns i so that the processes dont interrupt each other (while loop)
int	minishell_helper(char *input, char **env, t_vars *vars,
		char **commands)
{
	if (str_is_spaces_only(input))
		return (0);
	commands = ft_split_commands(input, "|");
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
	free(commands);
	commands = NULL;
	return (vars->i);
}

void	minishell(char *input, char **env, t_vars *vars, char **commands)
{
	int		i;
	int		status;

	i = 0;
	vars->exit_stat = 0;
	commands = NULL;
	status = 0;
	i = minishell_helper(input, env, vars, commands);
	if (i == 0)
		return ;
	close(vars->pipe_fd[0]);
	while (i-- > 0)
	{
		wait(&status);
		if (WIFEXITED(status))
			vars->exit_stat = WEXITSTATUS(status);
	}
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

void	run_shell(t_vars *vars, char **env)
{
	char	*input;
	char	**commands;

	commands = NULL;
	while (1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, signal_handler);
		input = readline("myshell> ");
		if (!ft_exit_ctrl_d(input))
		{
			vars->exit_stat = 1;
			if (commands)
				ft_free (commands);
			ft_free(vars->my_environ);
			ft_free_vars(vars);
			break ;
		}
		if (ft_strlen(input) != 0)
			add_history(input);
		if (ft_strlen(input) != 0)
			minishell(input, env, vars, commands);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, signal_handler);
	}
}

int	main(int ac, char **av, char **env)
{
	t_vars	vars;

	(void)ac;
	(void)av;
	setup_shell(&vars, env);
	run_shell(&vars, env);
	return (0);
}
