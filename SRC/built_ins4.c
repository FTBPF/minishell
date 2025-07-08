/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marada <marada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:37 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/08 17:29:51 by marada           ###   ########.fr       */
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
	exit(0);
}

// Helps determine the number of env variables that need to be unset.
// Used to adjust the size of the new environment variable array
// after unsetting the variables.
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
			if (ft_strncmp(commands[x], vars->my_environ[i], \
				ft_strlen(commands[x])) == 0)
				j++;
			x++;
		}
		i++;
	}
	return (j);
}

// Clears value from env variables, otherwise does nothing
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


static size_t	ft_tamnhoplavra(char const *s, char c)
{
	size_t	i;
	int		flag;
	
	flag = 0;
	i = 0;
	while (s[i] && (s[i] != c || flag == 1))
	{
		if (s[i] == '\"' || s[i] == '\'')
		{
			if (flag == 0)
			flag = 1;
			else
			flag = 0;
		}
		i++;
	}
	return (i);
}

static size_t	ft_ctp(char const *s, char c)
{
	size_t	i;
	int		flag;
	size_t	ctp;
	
	flag = 0;
	i = 0;
	ctp = 0;
	while (s[i] && s[i] == c)
	i++;
	while (s[i])
	{
		while (s[i] && (s[i] != c || flag == 1))
		{
			if (s[i] == '\"' || s[i] == '\'')
			{
				if (flag == 0)
				flag = 1;
				else
				flag = 0;
			}
			i++;
		}
		ctp++;
		while (s[i] && s[i] == c)
		i++;
	}
	return (ctp);
}

static char	**ft_putmatrix(char **matrix, char const *s, char c, size_t	ctp)
{
	size_t	i;
	int		flag;
	size_t	j;
	
	i = 0;
	flag = 0;
	j = 0;
	while (*s && *s == c)
	s++;
	while (ctp)
	{
		matrix[j] = (char *)malloc(sizeof(char) * (ft_tamnhoplavra(s, c) + 1));
		i = 0;
		while (*s && (*s != c || flag == 1))
		{
			if (s[i] == '\"' || s[i] == '\'')
			{
				if (flag == 0)
				flag = 1;
				else
				flag = 0;
			}
			matrix[j][i] = *s;
			i++;
			s++;
		}
		matrix[j][i] = '\0';
		while (*s && *s == c)
		s++;
		j++;
		ctp--;
	}
	matrix[j] = 0;
	return (matrix);
}

char	**ft_split_novo_e_melhorado(char const *s, char c)
{
	char	**matrix;

	if (!s)
		return (0);
	matrix = (char **)malloc(sizeof(char *) * (ft_ctp(s, c) + 1));
	if (!matrix || !s)
		return (0);
	matrix = ft_putmatrix(matrix, s, c, ft_ctp(s, c));
	return (matrix);	
}

int	check_cd_ex_uns(char **commands, t_vars *vars)
{
	char	**split_cmds;

	split_cmds = ft_split_novo_e_melhorado(commands[0], ' ');
	if (ft_strcmp(split_cmds[0], "cd") == 0)
		ft_cd(split_cmds, vars);
	else if (ft_strcmp(split_cmds[0], "exit") == 0 && split_cmds[2])
	{
		ft_putendl_fd("exit: too many arguments\n", 2);
		vars->exit_stat = 1;
	}
	else if (ft_strcmp(split_cmds[0], "exit") == 0)
		ft_exit(split_cmds);
	else if (ft_strcmp(split_cmds[0], "unset") == 0)
		ft_unset(vars, split_cmds);
	else if (ft_strcmp(split_cmds[0], "export") == 0 && split_cmds[1])
		ft_export(vars, split_cmds);
	else
	{
		ft_free(split_cmds);
		return (0);
	}
	ft_free(split_cmds);
	return (1);
}
