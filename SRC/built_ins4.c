/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:23 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:49:58 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_command_in_env(char *env_var, char **commands)
{
	int	x;
	int	flag;

	x = 1;
	flag = 1;
	while (commands[x])
	{
		if (ft_strncmp(commands[x], env_var, ft_strlen(commands[x])) == 0)
			flag = 0;
		x++;
	}
	return (flag);
}

void	ft_unset(t_vars *vars, char **commands)
{
	int		i;
	int		j;
	char	**new_environ;

	i = 0;
	j = 0;
	vars->num_env_vars = vars->num_env_vars - env_num(vars, commands);
	new_environ = malloc(sizeof(char *) * (vars->num_env_vars + 1));
	while (vars->my_environ[i] != NULL)
	{
		if (is_command_in_env(vars->my_environ[i], commands))
		{
			new_environ[j] = ft_strdup(vars->my_environ[i]);
			j++;
		}
		i++;
	}
	new_environ[j] = NULL;
	ft_free(vars->my_environ);
	vars->my_environ = new_environ;
}

static int	handle_cd_exit(char **cmds, t_vars *vars)
{
	if (ft_strcmp(cmds[0], "cd") == 0)
	{
		ft_cd(cmds, vars);
		return (1);
	}
	if (ft_strcmp(cmds[0], "exit") == 0 && cmds[1] && cmds[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		*exit_status() = 1;
		return (1);
	}
	if (ft_strcmp(cmds[0], "exit") == 0)
	{
		ft_exit(vars, cmds);
		return (1);
	}
	return (0);
}

static int	handle_export_unset(char **cmds, t_vars *vars)
{
	if (ft_strcmp(cmds[0], "unset") == 0)
	{
		ft_unset(vars, cmds);
		return (1);
	}
	if (ft_strcmp(cmds[0], "export") == 0 && cmds[1])
	{
		ft_export(vars, cmds);
		return (1);
	}
	return (0);
}

int	check_cd_ex_uns(char **commands, t_vars *vars)
{
	char	**cmds;
	int		result;

	if (vars->in_pipeline && (ft_strstr(commands[0], "export")
			|| ft_strstr(commands[0], "unset")))
		return (0);
	cmds = ft_split_novo_e_melhorado(commands[0], ' ');
	if (!cmds || !cmds[0])
	{
		if (cmds)
			ft_free(cmds);
		return (0);
	}
	result = handle_cd_exit(cmds, vars);
	if (!result)
		result = handle_export_unset(cmds, vars);
	ft_free(cmds);
	return (result);
}
