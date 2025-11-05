/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:23:05 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:26:46 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static char	*get_env_value(t_vars *vars, char *key)
{
	int		line_nbr;
	char	*value;

	line_nbr = find_env_line_nbr(vars, key);
	if (line_nbr != -1 && vars->my_environ[line_nbr])
		value = get_value_for_expand(vars->my_environ[line_nbr]);
	else
		value = ft_strdup("");
	return (value);
}

static char	*build_replaced_str(char *cmds, int j, int i, char *insert)
{
	char	*freeme;
	char	*result;

	freeme = NULL;
	ft_replace_helper2(cmds, j, NULL, &freeme);
	if (!freeme)
		return (NULL);
	if (i + j <= (int)ft_strlen(cmds))
		result = ft_strjoin_three(freeme, insert, &cmds[i + j]);
	else
		result = ft_strjoin_three(freeme, insert, "");
	free(freeme);
	return (result);
}

char	*replace_var(t_vars *vars, char *cmds, int j)
{
	int		i;
	char	*key;
	char	*val;
	char	*new_str;

	if (!cmds || j >= (int)ft_strlen(cmds))
		return (cmds);
	key = NULL;
	i = ft_replace_helper(cmds, j, &key);
	if (!key || i == 0)
		return (free(key), cmds);
	val = get_env_value(vars, key);
	if (!val)
		return (free(key), cmds);
	new_str = build_replaced_str(cmds, j, i, val);
	free(key);
	free(val);
	free(cmds);
	if (!new_str)
		return (cmds);
	return (new_str);
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
