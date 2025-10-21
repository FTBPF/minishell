/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:27 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/21 11:53:28 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Handles various combinations of NULL or empty strings, joining
// only the non-empty ones. If all three strings exist and are non-empty,
// calls ft_strjoin_three_help.
char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*str;

	if (!s1 && !s2 && !s3)
		return (NULL);
	else if ((!s1 || !*s1) && (s2 && s3))
		str = ft_strjoin(s2, s3);
	else if ((!s2 || !*s2) && (s1 && s3))
		str = ft_strjoin(s1, s3);
	else if (((!s3 || !*s3) && (s1 && s2)))
		str = ft_strjoin(s1, s2);
	else if (s1 && (!s2 || !*s2) && (!s3 || !*s3))
		str = ft_strdup(s1);
	else if (s2 && (!s1 || !*s1) && (!s3 || !*s3))
		str = ft_strdup(s2);
	else if (s3 && (!s1 || !*s1) && (!s2 || !*s2))
		str = ft_strdup(s3);
	else
	{
		str = NULL;
		str = ft_strjoin_three_help(s1, s2, s3, str);
	}
	return (str);
}

// Creates a new environment variable in the format "name=value" and
// adds it to the my_environ array. Reallocates the array to accommodate
// the new variable and increments num_env_vars.

void	add_env_var(t_vars *vars, char *name, char *value)
{
	int		i;
	char	**new_environ;
	char	*new_env_var = NULL;

	new_environ = malloc((vars->num_env_vars + 2) * sizeof(char *));
	if (!new_environ)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < vars->num_env_vars)
	{
		new_environ[i] = vars->my_environ[i];
		i++;
	}
	if (value)
		new_env_var = ft_strjoin_three(name, "=", value);
	else if (!value)
		new_env_var = ft_strjoin_three(name, "", "");
	new_environ[i] = new_env_var;
	new_environ[i + 1] = NULL;
	free(vars->my_environ);
	vars->my_environ = new_environ;
	vars->num_env_vars++;	
}

// Searches for the variable by name. If found, replaces its value.
// If not found, calls add_env_var to create a new variable.

void	modify_env_var(t_vars *vars, char *name, char *new_value)
{
	int		i;
	int		name_len;
	char	*new_env_var;

	name_len = ft_strlen(name);
	i = 0;
	while (vars->my_environ[i])
	{
		if (new_value == NULL)
		{
			free(vars->my_environ[i]);
			new_env_var = ft_strjoin_three(name, "", "");
			vars->my_environ[i] = new_env_var;
			return ;
		}
		if (ft_strncmp(vars->my_environ[i], name, name_len) == 0)
		{
			free(vars->my_environ[i]);
			new_env_var = ft_strjoin_three(name, "=", new_value);
			vars->my_environ[i] = new_env_var;
			return ;
		}
		i++;
	}
	add_env_var(vars, name, new_value);
}

// Searches in my_environ by the name. If found returns a pointer 
// to the value part of the environment variable, returns NULL if not found

char	*get_env_var(t_vars *vars, char *name)
{
	int	i;
	int	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while (vars->my_environ[i])
	{
		if (ft_strncmp(vars->my_environ[i], name, name_len) == 0
			&& vars->my_environ[i][name_len] == '=')
			return (vars->my_environ[i] + name_len + 1);
		i++;
	}
	return (NULL);
}

// Creates a deep copy of the environment variables array and stores it
// in vars->my_environ. Sets vars->num_env_vars to the count of variables.

void	copy_environ(char **environ, t_vars *vars)
{
	int	i;
	int	j;

	i = 0;
	while (environ[i] != NULL)
		i++;
	vars->num_env_vars = i;
	vars->my_environ = malloc(sizeof(char *) * (i + 1));
	if (!vars->my_environ)
		return ;
	i = 0;
	while (environ[i] != NULL)
	{
		vars->my_environ[i] = ft_strdup(environ[i]);
		if (vars->my_environ[i] == NULL)
		{
			j = 0;
			while (j < i)
				free(vars->my_environ[j++]);
			free(vars->my_environ);
			return ;
		}
		i++;
	}
	vars->my_environ[i] = NULL;
}
