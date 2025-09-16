/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:43:06 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/16 16:04:35 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_replace_helper2(char *commands, int j, char *tmp, char **freee)
{
	int	k;

	free(*freee);
	*freee = NULL;
	k = 0;
	tmp = ft_strdup("");
	while (k < j - 1 && commands[k])
	{
		*freee = ft_strjoin_char(tmp, commands[k]);
		free(tmp);
		tmp = ft_strdup(*freee);
		k++;
	}
	free(tmp);
}

int	ft_replace_helper(char *commands, int j, char **tmp)
{
	int		i;
	char	*fre;

	fre = NULL;
	*tmp = ft_strdup("");
	i = 0;
	while (commands[j + i] && ((commands[j + i] >= 'a' && commands[j
				+ i] <= 'z') || (commands[j + i] >= 'A' && commands[j
				+ i] <= 'Z')))
	{
		fre = *tmp;
		*tmp = ft_strjoin_char(*tmp, commands[j + i]);
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
	line_nbr = find_env_line_nbr(vars, tmp);
	if (line_nbr != -1 && vars->my_environ[line_nbr])
		tmp2 = get_value_for_expand(vars->my_environ[line_nbr]);
	else
		tmp2 = ft_strdup("");
	ft_replace_helper2(commands, j, NULL, &tmp);
	if (tmp2 && commands)
		tmp3 = ft_strjoin_three(tmp, tmp2, &commands[i + j]);
	if (tmp)
		free(tmp);
	if (tmp2)
		free(tmp2);
	free(commands);
	return (tmp3);
}

void	var_expander(t_vars *vars, char **commands)
{
	int	i;

	i = 0;
	while (commands[i])
	{
		if (strchr(commands[i], '$'))
		{
			ft_expander_helper2(commands, vars, i);
		}
		i++;
	}
}
