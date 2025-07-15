/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:15:18 by marada            #+#    #+#             */
/*   Updated: 2025/07/15 16:04:26 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	new_improved_strcpy(char *dst, char *src)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = -1;
	c = '+';
	while (src[++j])
	{
		if (src[j] == '\"' || src[j] == '\'')
		{
			if (src[j] == c)
				c = '+';
			else if (c == '+')
				c = src[j];
			else
				dst[i++] = src[j];
		}
		else
			dst[i++] = src[j];
	}
	dst[i] = '\0';
}

int	get_value_helper(char *str, int j)
{
	int		x;
	char	c;

	x = 0;
	c = '+';
	while (str[++j])
	{
		if (str[j] == '\"' || str[j] == '\'')
		{
			if (str[j] == c)
				c = '+';
			else if (c == '+')
				c = str[j];
			else
				x++;
		}
		else
			x++;
	}
	return (x);
}

char	*new_get_value(char *str)
{
	int		i;
	char	*value;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (NULL);
	i++;
	value = malloc(sizeof(char) * (get_value_helper(str, i - 1) + 1));
	if (!value)
		exit(1);
	new_improved_strcpy(value, str + i);
	return (value);
}
