/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:23 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/22 13:20:25 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		ft_printf("%s\n", pwd);
		free(pwd);
	}
	else
		perror("pwd");
	g_exit_status = 0;
}

int	env_num(t_vars *vars, char **commands)
{
	int	i;
	int	j;
	int	x;

	i = 0;
	j = 0;
	while (vars->my_environ[i] != NULL)
	{
		x = 1;
		while (commands[x])
		{
			if (ft_strncmp(commands[x], vars->my_environ[i],
					ft_strlen(commands[x])) == 0)
				j++;
			x++;
		}
		i++;
	}
	return (j);
}

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

int	check_cd_ex_uns(char **commands, t_vars *vars)
{
	char	**split_cmds;

	if (vars->in_pipeline && (ft_strstr(commands[0], "export")
			|| ft_strstr(commands[0], "unset")))
		return (0);
	split_cmds = ft_split_novo_e_melhorado(commands[0], ' ');
	if (!split_cmds || !split_cmds[0])
		return (ft_free(split_cmds), 0);
	if (ft_strcmp(split_cmds[0], "cd") == 0)
		ft_cd(split_cmds, vars);
	else if (ft_strcmp(split_cmds[0], "exit") == 0 && split_cmds[1]
		&& split_cmds[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		g_exit_status = 1;
	}
	else if (ft_strcmp(split_cmds[0], "exit") == 0)
		ft_exit(split_cmds);
	else if (ft_strcmp(split_cmds[0], "unset") == 0)
		ft_unset(vars, split_cmds);
	else if (ft_strcmp(split_cmds[0], "export") == 0 && split_cmds[1])
		ft_export(vars, split_cmds);
	else
		return (ft_free(split_cmds), 0);
	return (ft_free(split_cmds), 1);
}
