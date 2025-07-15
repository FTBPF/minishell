/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins7.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marada <marada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:06:41 by marada            #+#    #+#             */
/*   Updated: 2025/07/13 19:09:03 by marada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		perror(path);
}
