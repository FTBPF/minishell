/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins8.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:26:23 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/16 16:27:59 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	change_directory(char *path, t_vars *vars)
{
	char	*old_pwd;
	char	*new_pwd;
	char	*temp;

	old_pwd = get_env_var(vars, "PWD");
	temp = old_pwd;
	if (chdir(path) == 0)
	{
		new_pwd = getcwd(NULL, 0);
		modify_env_var(vars, "OLDPWD", temp);
		modify_env_var(vars, "PWD", new_pwd);
		free(new_pwd);
	}
	else
	{
		ft_putstr_fd("minishell: cd: ", 2);
		g_exit_status = 1;
		perror(path);
	}
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
	g_exit_status = 0;
	exit(g_exit_status);
}
