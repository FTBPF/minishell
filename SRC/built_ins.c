/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:19 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/14 18:11:24 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_if_builtin(t_vars *vars)
{
	char	*cmd;

	cmd = vars->cmd_flags[0];
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	run_builtin(t_vars *vars)
{
	char	*cmd;

	cmd = vars->cmd_flags[0];
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(vars->cmd_flags));
	if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(vars->cmd_flags, vars));
	if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(vars, vars->cmd_flags));
	if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(vars, vars->cmd_flags));
	if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(vars));
	if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(vars->cmd_flags));
	return ;
}

void	ft_echo2(char **commands, int i)
{
	while (commands[i])
	{
		ft_printf("%s", commands[i]);
		if (commands[i + 1])
			ft_printf(" ");
		i++;
	}
}

void	ft_echo(char **commands)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (commands[1] && check_flag_n(commands[1]) == 1)
	{
		n_flag = 1;
		i++;
	}
	ft_echo2(commands, i);
	if (!n_flag)
		ft_printf("\n");
	exit(0);
}

void	ft_cd(char **commands, t_vars *vars)
{
	char	*home;
	char	*old_pwd;

	home = get_env_var(vars, "HOME");
	old_pwd = get_env_var(vars, "OLDPWD");
	if (!commands[1])
	{
		if (home)
			change_directory(home, vars);
		else
			ft_printf("minishell: cd: HOME not set\n");
	}
	else if (ft_strcmp(commands[1], "-") == 0)
	{
		old_pwd = get_env_var(vars, "OLDPWD");
		if (old_pwd)
			change_directory(old_pwd, vars);
		else
			ft_printf("minishell: cd: OLDPWD not set\n");
	}
	else
		change_directory(commands[1], vars);
}
