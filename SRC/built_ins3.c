/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:31 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/22 17:28:12 by frteixei         ###   ########.fr       */
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

static void	print_exported_vars(t_vars *vars)
{
	int		i;
	char	**sorted_env;
	int		j;
	char	*temp;
	char	*equals;
	char	*name;
	char	*value;

	i = 0;
	sorted_env = malloc(sizeof(char *) * (vars->num_env_vars + 1));
	for (i = 0; i < vars->num_env_vars; i++)
		sorted_env[i] = ft_strdup(vars->my_environ[i]);
	sorted_env[i] = NULL;
	for (i = 0; i < vars->num_env_vars - 1; i++)
	{
		for (j = 0; j < vars->num_env_vars - i - 1; j++)
		{
			if (ft_strcmp(sorted_env[j], sorted_env[j + 1]) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
		}
	}
	for (i = 0; i < vars->num_env_vars; i++)
	{
		equals = ft_strchr(sorted_env[i], '=');
		if (equals)
		{
			*equals = '\0';
			name = sorted_env[i];
			value = equals + 1;
			ft_printf("declare -x %s=\"%s\"\n", name, value);
			*equals = '=';
		}
		else
			ft_printf("declare -x %s\n", sorted_env[i]);
	}
	ft_free(sorted_env);
}

static int	is_valid_identifier(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*get_var_name_fixed(char *str)
{
	int		i;
	char	*name;

	i = 0;
	if (!str || str[0] == '=')
	{
		g_exit_status = 1;
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(str ? str : "", 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (NULL);
	}
	if (!is_valid_identifier(str))
	{
		g_exit_status = 1;
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(str, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (NULL);
	}
	while (str[i] && str[i] != '=')
		i++;
	name = malloc(sizeof(char) * (i + 1));
	if (!name)
		exit(EXIT_FAILURE);
	ft_strlcpy(name, str, i + 1);
	return (name);
}

// FIXED EXPORT FUNCTION
void	ft_export(t_vars *vars, char **split_cmds)
{
	int		i;
	char	*name;
	char	*value;

	i = 1;
	if (!split_cmds[1])
	{
		g_exit_status = 0;
		print_exported_vars(vars);
		return ;
	}
	while (split_cmds[i])
	{
		name = get_var_name_fixed(split_cmds[i]);
		if (!name)
		{
			i++;
			continue ;
		}
		value = new_get_value(split_cmds[i]);
		if (value)
		{
			modify_env_var(vars, name, value);
			free(value);
		}
		else
		{
			if (find_env_line_nbr(vars, name) == -1)
				add_env_var(vars, name, "");
		}
		free(name);
		i++;
	}
}
