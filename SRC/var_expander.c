/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:23:05 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/03 15:23:48 by frteixei         ###   ########.fr       */
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
	if (!*tmp)
		return (0);
	i = 0;
	while (commands[j + i] && ((commands[j + i] >= 'a' && commands[j
					+ i] <= 'z') || (commands[j + i] >= 'A' && commands[j
					+ i] <= 'Z')))
	{
		fre = *tmp;
		*tmp = ft_strjoin_char(*tmp, commands[j + i]);
		if (!*tmp)
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
	char	*freeme;

	if (!commands || j >= (int)ft_strlen(commands))
		return (commands);
	tmp = NULL;
	i = ft_replace_helper(commands, j, &tmp);
	if (!tmp || i == 0)
	{
		if (tmp)
			free(tmp);
		return (commands);
	}
	line_nbr = find_env_line_nbr(vars, tmp);
	if (line_nbr != -1 && vars->my_environ[line_nbr])
		tmp2 = get_value_for_expand(vars->my_environ[line_nbr]);
	else
		tmp2 = ft_strdup("");
	if (!tmp2)
	{
		free(tmp);
		return (commands);
	}
	freeme = NULL;
	ft_replace_helper2(commands, j, NULL, &freeme);
	if (!freeme)
	{
		free(tmp);
		free(tmp2);
		return (commands);
	}
	if (i + j <= (int)ft_strlen(commands))
		tmp3 = ft_strjoin_three(freeme, tmp2, &commands[i + j]);
	else
		tmp3 = ft_strjoin_three(freeme, tmp2, "");
	free(freeme);
	free(tmp);
	free(tmp2);
	free(commands);
	return (tmp3);
}

void	var_expander(t_vars *vars, char **commands)
{
	int	i;

	if (!vars || !commands)
		return ;
	i = 0;
	while (commands[i])
	{
		if (strchr(commands[i], '$'))
			ft_expander_helper2(commands, vars, i);
		i++;
	}
}
