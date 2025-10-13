/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:43:12 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/13 16:14:24 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Extracts the parts before and after $?, converts g_exit_status
// to string, and joins them together. Frees the original command.

char	*replace_exit_status(char *commands, int j)
{
	char	*temp;
	char	*temp2;

	temp = NULL;
	temp2 = NULL;
	if (j >= 2)
		temp = ft_substr(commands, 0, j - 2);
	else
		temp = ft_strdup("");
	temp2 = ft_substr(commands, j, ft_strlen(commands) - j);
	free(commands);
	commands = ft_strjoin_three(temp, ft_itoa(g_exit_status), temp2);
	free(temp);
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

// Iterates through command character by character, tracking quote state.
// When $ is found outside single quotes, expands the variable or exit status.
// Handles quote state to prevent expansion in single quotes.

void	ft_expander_helper2(char **commands, t_vars *vars, int i)
{
	int	j;
	int	in_quotes;
	int	in_squotes;

	j = 0;
	in_quotes = -1;
	in_squotes = -1;
	while (commands[i][j])
	{
		if (in_squotes == -1 && check_if_exit_stat(commands, i, j))
			continue ;
		else if (in_squotes == -1 && commands[i][j] == '$' && commands[i][j
			+ 1] != ' ' && commands[i][j + 1] != '\0' && commands[i][j
			+ 1] != '\"')
		{
			commands[i] = replace_var(vars, commands[i], j + 1);
			if (!commands[i])
				return ;
		}
		if (commands[i][j] == '"' && in_squotes == -1 && in_quotes == -1)
			in_quotes *= -1;
		if (commands[i][j] == 39 && in_quotes == -1 && in_squotes == -1)
			in_squotes *= -1;
		j++;
	}
}
