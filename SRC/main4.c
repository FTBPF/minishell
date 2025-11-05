/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:39:57 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:40:23 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_pipe_syntax(char *input)
{
	int	i;
	int	len;

	if (str_is_spaces_only(input))
		return (1);
	if (input[0] == '|')
	{
		ft_printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	len = ft_strlen(input);
	i = len - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t'))
		i--;
	if (i >= 0 && input[i] == '|')
	{
		ft_printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

int	validate_commands(char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		if (str_is_spaces_only(cmds[i]))
		{
			ft_printf("minishell: syntax error near unexpected token `|'\n");
			ft_free(cmds);
			return (0);
		}
		i++;
	}
	return (1);
}
