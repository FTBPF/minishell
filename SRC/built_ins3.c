/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:31 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/18 09:44:47 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_env(t_vars *vars)
{
	int	i;

	i = 0;
	while (vars->my_environ[i])
	{
		ft_printf("%s\n", vars->my_environ[i]);
		i++;
	}
	g_exit_status = 0;
	exit(g_exit_status);
}

char	*ft_export_error(char *str)
{
	g_exit_status = 1;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (NULL);
}

char	*get_var_name(char *str)
{
	int		i;
	char	*name;

	i = 0;
	if (str[i] == '=' || ft_isdigit(str[1]))
		return (ft_export_error(str));
	while (str[i] && str[i] != '=' && str[i] != '-')
		i++;
	if (str[i] == '-')
		return (ft_export_error(str));
	else if (str[i] != '=')
		return (NULL);
	name = malloc(sizeof(char) * (i + 1));
	if (!name)
	{
		g_exit_status = 1;
		exit(g_exit_status);
	}
	ft_strlcpy(name, str, i + 1);
	return (name);
}

char	*get_value(char *str)
{
	int		i;
	int		j;
	char	*value;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (NULL);
	i++;
	j = i;
	while (str[j])
		j++;
	value = malloc(sizeof(char) * (j - i + 1));
	if (!value)
	{
		g_exit_status = 1;
		exit(g_exit_status);
	}
	ft_strlcpy(value, str + i, j - i + 1);
	return (value);
}

void	ft_export(t_vars *vars, char **split_cmds)
{
	int		i;
	char	*name;
	char	*value;

	i = 1;
	if (split_cmds[i])
	{
		while (split_cmds[i])
		{
			name = get_var_name(split_cmds[i]);
			value = new_get_value(split_cmds[i]);
			if (name && value)
				modify_env_var(vars, name, value);
			else if (name)
				add_env_var(vars, name, "");
			free(name);
			free(value);
			i++;
		}
	}
	else
	{
		ft_env(vars);
		g_exit_status = 0;
	}
}
