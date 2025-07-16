/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:31 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/16 15:54:40 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	ft_exit_error(char *msg, char *arg, int status)
{
	g_exit_status = status;
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(msg, 2);
	exit(status);
}

void	ft_exit(char **split_cmds)
{
	int		i;
	char	*split;
	char	*clean_str;

	i = 0;
	split = NULL;
	clean_str = NULL;
	while (split_cmds[i])
		i++;
	if (i == 1)
		g_exit_status = EXIT_SUCCESS;
	else
	{
		if (i > 2)
			return (g_exit_status = 1,
				ft_putstr_fd("minishell: exit: too many arguments\n", 2));
		check_if_exit_stat(&split_cmds[1], 0, 0);
		split = remove_quotes_from_string(split_cmds[1]);
		if (!split || !split[0])
			ft_exit_error(": numeric argument required\n", split_cmds[1], 2);
		clean_str = ft_strdup(split);
		free(split);
		if (!ft_is_valid_number(clean_str))
			ft_exit_error(": numeric argument required\n", clean_str, 2);
		g_exit_status = ft_atoi(clean_str);
		free(clean_str);	
	}
	ft_printf("exit\n");
	exit((unsigned char)g_exit_status);
}

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
	int		equal_flag;
	char	*name;

	i = 0;
	equal_flag = 0;
	if (str[i] == '=' || ft_isdigit(str[1]))
		return (ft_export_error(str));
	while (str[i] && str[i] != '=' && str[i] != '-')
		i++;
	if (str[i] == '=' && str[i + 1] != '\0')
		equal_flag = 1;
	else if (str[i] == '-')
		return (ft_export_error(str));
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
		g_exit_status = 1;
		exit(g_exit_status);
	}
}
