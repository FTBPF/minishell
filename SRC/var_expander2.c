/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:23:09 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 15:02:34 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*replace_exit_status(char *commands, int j)
{
	char	*temp;
	char	*temp2;
	char	*num;

	temp = NULL;
	temp2 = NULL;
	if (j >= 2)
		temp = ft_substr(commands, 0, j - 2);
	else
		temp = ft_strdup("");
	temp2 = ft_substr(commands, j, ft_strlen(commands) - j);
	num = ft_itoa(*exit_status());
	free(commands);
	commands = ft_strjoin_three(temp, num, temp2);
	free(temp);
	free(num);
	free(temp2);
	return (commands);
}

int	check_if_exit_stat(char **commands, int i, int j)
{
	if (commands[i][j] == '$' && commands[i][j + 1] == '?')
	{
		commands[i] = replace_exit_status(commands[i], j + 2);
		return (1);
	}
	return (0);
}

void	ft_expander_helper2(char **commands, t_vars *vars, int i)
{
	int	j;
	int	in_quotes;
	int	in_squotes;
	int	len;

	j = 0;
	in_quotes = -1;
	in_squotes = -1;
	while (commands[i] && commands[i][j])
	{
		len = ft_strlen(commands[i]);
		if (j >= len)
			break ;
		if (in_squotes == -1 && commands[i][j] == '$' && (j + 1) < len
			&& commands[i][j + 1] == '?')
		{
			commands[i] = replace_exit_status(commands[i], j + 2);
			if (!commands[i])
				return ;
			j = 0;
			continue ;
		}
		else if (in_squotes == -1 && commands[i][j] == '$' && (j + 1) < len
			&& commands[i][j + 1] != ' ' && commands[i][j + 1] != '\0'
			&& commands[i][j + 1] != '\"')
		{
			commands[i] = replace_var(vars, commands[i], j + 1);
			if (!commands[i])
				return ;
			j = 0;
			continue ;
		}
		if (commands[i][j] == '"' && in_squotes == -1)
			in_quotes *= -1;
		if (commands[i][j] == 39 && in_quotes == -1)
			in_squotes *= -1;
		j++;
	}
}
