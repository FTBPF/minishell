/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:26 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/22 13:21:27 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_pipe_syntax(char *input)
{
	if (str_is_spaces_only(input))
		return (1);
	if (input[0] == '|')
	{
		ft_printf("minishell: syntax error near unexpected token |'\n");
		return (1);
	}
	return (0);
}

static int	validate_commands(char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		if (str_is_spaces_only(cmds[i]))
		{
			ft_printf("minishell: syntax error near unexpected token |'\n");
			ft_free(cmds);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	handle_cd_and_expansion(t_vars *vars, char **cmds, char *input)
{
	if (ft_strchr(input, '$'))
		var_expander(vars, cmds);
	if (check_cd_ex_uns(cmds, vars))
	{
		ft_free_vars(vars);
		ft_free(cmds);
		return (1);
	}
	return (0);
}

int	setup_commands(char *input, t_vars *vars, char ***commands)
{
	int	invalid;

	invalid = check_pipe_syntax(input);
	if (invalid)
		return (0);
	*commands = ft_split_commands(input, "|");
	if (!*commands)
		return (0);
	if (!validate_commands(*commands))
		return (0);
	vars->in_pipeline = ((*commands)[1] != NULL);
	if (handle_cd_and_expansion(vars, *commands, input))
		return (0);
	here_doc(vars, *commands);
	return (1);
}

int	ft_has_invalid_pipe(char **commands)
{
	int	i;
	int	j;

	if (!commands || !commands[0])
		return (1);
	if (commands[0][0] == '|')
		return (1);
	i = 0;
	while (commands[i])
	{
		j = 0;
		while (commands[i][j])
		{
			if (commands[i][j] != ' ' && commands[i][j] != '\t'
				&& commands[i][j] != '|')
				break ;
			j++;
		}
		if (commands[i][j] == '\0')
			return (1);
		i++;
	}
	return (0);
}

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
