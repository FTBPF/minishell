/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 15:11:58 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/09 16:23:59 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Performs command setup:
// * Checks for whitespace-only input
// * Splits input by pipes
// * Marks if in pipeline
// * Expands variables
// * Handles parent-only built-ins
// * Processes heredocs

static int	setup_commands(char *input, t_vars *vars, char ***commands)
{
	if (str_is_spaces_only(input))
		return (0);
	*commands = ft_split_commands(input, "|");
	if (!*commands)
		return (0);
	vars->in_pipeline = ((*commands)[1] != NULL);
	if (ft_strchr(input, '$'))
		var_expander(vars, *commands);
	if (check_cd_ex_uns(*commands, vars))
	{
		ft_free_vars(vars);
		ft_free(*commands);
		return (0);
	}
	here_doc(vars, *commands);
	return (1);
}

// Iterates through all commands, executing each by calling
// first_process. Frees each command string after execution.
// Initializes pipeline state variables.

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

// Waits for all child processes to complete. If a process exited
// normally, stores its exit status. If terminated by signal,
// stores 0. Returns the last collected status.

static int	collect_status(void)
{
	int	status;
	int	last_status;

	last_status = 0;
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 0;
	}
	return (last_status);
}

// Waits for all child processes to complete. If a process exited
// normally, stores its exit status. If terminated by signal,
// stores 0. Returns the last collected status.

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
