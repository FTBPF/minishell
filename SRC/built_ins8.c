/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins8.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:47 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:42:11 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		*exit_status() = 1;
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
	*exit_status() = 0;
}

void	ctp_helper(char const *s, char c, char *d, size_t *i)
{
	while (s[*i] && (s[*i] != c || *d != '+'))
	{
		if (s[*i] == '\"' || s[*i] == '\'')
		{
			if (s[*i] == *d)
				*d = '+';
			else if (*d == '+')
				*d = s[*i];
		}
		(*i)++;
	}
}

void	put_matrix_helper(char const **s, char c, char *d, char *str)
{
	int	i;

	i = 0;
	while (**s && (**s != c || *d != '+'))
	{
		if (**s == '\"' || **s == '\'')
		{
			if (**s == *d)
				*d = '+';
			else if (*d == '+')
				*d = **s;
		}
		str[i] = **s;
		i++;
		(*s)++;
	}
	str[i] = '\0';
}
