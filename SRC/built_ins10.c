/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins10.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 13:58:44 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 13:58:56 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sort_env(char **env, int count)
{
	char	*temp;
	int		i;
	int		j;

	i = -1;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
		}
	}
}

static char	**copy_and_sort_env(t_vars *vars)
{
	char	**sorted_env;
	int		i;

	sorted_env = malloc(sizeof(char *) * (vars->num_env_vars + 1));
	if (!sorted_env)
		return (NULL);
	i = -1;
	while (++i < vars->num_env_vars)
		sorted_env[i] = ft_strdup(vars->my_environ[i]);
	sorted_env[i] = NULL;
	sort_env(sorted_env, vars->num_env_vars);
	return (sorted_env);
}

static void	print_sorted_env(char **sorted_env, int count)
{
	char	*equals;
	char	*name;
	char	*value;
	int		i;

	i = -1;
	while (++i < count)
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
}

void	print_exported_vars(t_vars *vars)
{
	char	**sorted_env;

	sorted_env = copy_and_sort_env(vars);
	if (!sorted_env)
		return ;
	print_sorted_env(sorted_env, vars->num_env_vars);
	ft_free(sorted_env);
}
