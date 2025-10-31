/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:23:05 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/31 15:53:13 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_replace_helper2(char *commands, int j, char *tmp, char **freee)
{
	int		k;
	char	*result;
	char	*new_result;

	(void)tmp;
	if (*freee)
	{
		free(*freee);
		*freee = NULL;
	}
	result = ft_strdup("");
	if (!result)
		return ;
	k = 0;
	while (k < j - 1 && commands[k])
	{
		new_result = ft_strjoin_char(result, commands[k]);
		free(result);
		result = new_result;
		if (!result)
			return ;
		k++;
	}
	*freee = result;
}

int	ft_replace_helper(char *commands, int j, char **tmp)
{
	int		i;
	char	*fre;

	fre = NULL;
	*tmp = ft_strdup("");
	if (!*tmp) // ADD NULL CHECK
		return (0);
	i = 0;
	while (commands[j + i] && ((commands[j + i] >= 'a' && commands[j
				+ i] <= 'z') || (commands[j + i] >= 'A' && commands[j
				+ i] <= 'Z')))
	{
		fre = *tmp;
		*tmp = ft_strjoin_char(*tmp, commands[j + i]);
		if (!*tmp) // ADD NULL CHECK
		{
			free(fre);
			fre = NULL;
			return (0);
		}
		i++;
		free(fre);
	}
	return (i);
}

char	*replace_var(t_vars *vars, char *commands, int j)
{
	int		i;
	int		line_nbr;
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	tmp = NULL;
	tmp3 = NULL;
	i = ft_replace_helper(commands, j, &tmp);
	if (!tmp)
		return (commands);
	if (i == 0)
	{
		free(tmp);
		tmp = NULL;
		return (commands);
	}
	line_nbr = find_env_line_nbr(vars, tmp);
	if (line_nbr != -1 && vars->my_environ[line_nbr])
		tmp2 = get_value_for_expand(vars->my_environ[line_nbr]);
	else
		tmp2 = ft_strdup("");
	if (!tmp2) // ADD NULL CHECK
	{
		free(tmp);
		tmp = NULL;
		return (commands);
	}
	ft_replace_helper2(commands, j, NULL, &tmp);
	if (!tmp) // ADD NULL CHECK after ft_replace_helper2
	{
		free(tmp2);
		tmp2 = NULL;
		return (commands);
	}
	if (tmp2 && commands)
		tmp3 = ft_strjoin_three(tmp, tmp2, &commands[i + j]);
	free(tmp);
	tmp = NULL;
	free(tmp2);
	tmp2 = NULL;
	free(commands);
	commands = NULL;
	return (tmp3);
}

void	var_expander(t_vars *vars, char **commands)
{
	int	i;

	if (!vars || !commands) // ADD NULL CHECK
		return ;
	i = 0;
	while (commands[i])
	{
		if (strchr(commands[i], '$'))
			ft_expander_helper2(commands, vars, i);
		i++;
	}
}
