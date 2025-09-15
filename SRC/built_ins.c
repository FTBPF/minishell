/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:19 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/15 16:29:44 by frteixei         ###   ########.fr       */
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
	else if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(vars->cmd_flags, vars));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(vars, vars->cmd_flags));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(vars, vars->cmd_flags));
	else if (ft_strcmp(cmd, "env") == 0)
		return (ft_env(vars));
	else if (ft_strcmp(cmd, "exit") == 0)
		return (ft_exit(vars->cmd_flags));
	else
		g_exit_status = 1;
	if (g_exit_status == -1)
		g_exit_status = 0;
	return ;
}

static int	handle_cd_special_cases(char **commands, t_vars *vars)
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
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	if (ft_strcmp(commands[1], "-") == 0)
	{
		if (old_pwd)
			change_directory(old_pwd, vars);
		else
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (1);
	}
	return (0);
}

void	ft_cd(char **commands, t_vars *vars)
{
	if (commands[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		g_exit_status = 1;
	}
	if (handle_cd_special_cases(commands, vars))
		return ;
	change_directory(commands[1], vars);
}
