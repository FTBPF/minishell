/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:23:09 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:33:12 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_exit_status_case(char **cmds, int i, int *j)
{
	int	len;

	len = ft_strlen(cmds[i]);
	if ((*j + 1) < len && cmds[i][*j + 1] == '?')
	{
		cmds[i] = replace_exit_status(cmds[i], *j + 2);
		if (!cmds[i])
			return (0);
		*j = 0;
		return (1);
	}
	return (-1);
}

static int	handle_var_case(char **cmds, t_vars *vars, int i, int *j)
{
	int	len;

	len = ft_strlen(cmds[i]);
	if ((*j + 1) < len && cmds[i][*j + 1] != ' ' && cmds[i][*j + 1] != '\0'
		&& cmds[i][*j + 1] != '\"')
	{
		cmds[i] = replace_var(vars, cmds[i], *j + 1);
		if (!cmds[i])
			return (0);
		*j = 0;
		return (1);
	}
	return (-1);
}

static void	toggle_quote_state(char c, int *in_quotes, int *in_squotes)
{
	if (c == '"' && *in_squotes == -1)
		*in_quotes *= -1;
	if (c == '\'' && *in_quotes == -1)
		*in_squotes *= -1;
}

void	ft_expander_helper2(char **cmds, t_vars *vars, int i)
{
	int	j;
	int	in_quotes;
	int	in_squotes;
	int	res;

	j = 0;
	in_quotes = -1;
	in_squotes = -1;
	while (cmds[i] && cmds[i][j])
	{
		if (in_squotes == -1 && cmds[i][j] == '$')
		{
			res = handle_exit_status_case(cmds, i, &j);
			if (res == 0)
				return ;
			if (res == 1)
				continue ;
			res = handle_var_case(cmds, vars, i, &j);
			if (res == 0)
				return ;
			if (res == 1)
				continue ;
		}
		toggle_quote_state(cmds[i][j++], &in_quotes, &in_squotes);
	}
}

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
