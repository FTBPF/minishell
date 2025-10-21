/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins9.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:57:29 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/21 11:52:21 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Checks if the string is a valid variable name:
// - Must start with a letter or underscore
// - Can contain letters, digits, and underscores before '='

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

static void	print_export_error(char *str)
{
	g_exit_status = 1;
	ft_putstr_fd("minishell: export: `", 2);
	if (str)
		ft_putstr_fd(str, 2);
	else
		ft_putstr_fd("", 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

// Validates that the string is a valid identifier and extracts
// the portion before the '=' sign. Prints error if invalid.

static char	*get_var_name_fixed(char *str)
{
	int		i;
	char	*name;

	i = 0;
	if (!str || str[0] == '=')
	{
		print_export_error(str);
		return (NULL);
	}
	if (!is_valid_identifier(str))
	{
		print_export_error(str);
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

// Extracts the variable name and value (if present) from the argument.
// If a value is provided, modifies the variable.

static void	handle_export_var(t_vars *vars, char *arg)
{
	char	*name;
	char	*value;

	name = get_var_name_fixed(arg);
	if (!name)
		return ;
	value = new_get_value(arg);
	if (value)
	{
		modify_env_var(vars, name, value);
		free(value);
	}
	else
		modify_env_var(vars, name, NULL);
	free(name);
}

// With no arguments, prints all exported variables in sorted order.
// With arguments, processes each one by calling handle_export_var
// to add or modify environment variables.

void	ft_export(t_vars *vars, char **split_cmds)
{
	int	i;

	i = 1;
	if (!split_cmds[1])
	{
		g_exit_status = 0;
		print_exported_vars(vars);
		return ;
	}
	while (split_cmds[i])
	{
		handle_export_var(vars, split_cmds[i]);
		i++;
	}
}
