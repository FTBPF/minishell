/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:32:57 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:33:16 by frteixei         ###   ########.fr       */
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
