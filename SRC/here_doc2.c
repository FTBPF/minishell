/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:59 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/07 19:42:00 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_open_helper(int *i, char *commands)
{
	char	current_quote;
	int		in_quotes;

	in_quotes = -1;
	current_quote = '\0';
	while (commands[*i])
	{
		if (in_quotes == -1 && (commands[*i] == ' ' || commands[*i] == '<' || \
			commands[*i] == '>'))
			break ;
		if ((commands[*i] == '\'' || commands[*i] == '\"')
			&& (in_quotes == -1 || current_quote == commands[*i]))
		{
			in_quotes *= -1;
			if (in_quotes == 1)
				current_quote = commands[*i];
			else
				current_quote = '\0';
		}
		(*i)++;
	}
}

void	ft_aspas_helper(int len, int *i, char *new_str, char *commands)
{
	char	current_quote;
	int		in_quotes;
	int		j;

	j = 0;
	in_quotes = -1;
	current_quote = '\0';
	while (j < len)
	{
		if ((in_quotes == 1 && current_quote != commands[j])
			|| (commands[j] != '\'' && commands[j] != '\"'))
			new_str[(*i)++] = commands[j];
		if ((commands[j] == '\'' || commands[j] == '\"')
			&& (in_quotes == -1 || current_quote == commands[j]))
		{
			in_quotes *= -1;
			if (in_quotes == 1)
				current_quote = commands[j];
			else
				current_quote = '\0';
		}
		j++;
	}
}

char	*ft_strndup_aspas(char *commands, int len)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = (char *)malloc(len + 1);
	if (!new_str && !commands)
		return (NULL);
	ft_aspas_helper(len, &i, new_str, commands);
	new_str[i] = '\0';
	return (new_str);
}
