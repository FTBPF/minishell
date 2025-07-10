/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marada <marada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:15:18 by marada            #+#    #+#             */
/*   Updated: 2025/07/10 19:06:03 by marada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	new_improved_strcpy(char *dst, char *src)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	j = 0;
	c = '+';
	while (src[j])
	{
		if (src[j] == '\"' || src[j] == '\'')
		{
			if (src[j] == c)
				c = '+';
			else if(c == '+')
				c = src[j];
			else
			{
				dst[i] = src[j];
				i++;
			}
		}
		else
		{
			dst[i] = src[j];
			i++;
		}
		j++;
	}
	dst[i] = '\0';
}

char	*new_get_value(char *str)
{
	int		i;
	int		x;
	int		j;
	char	c;
	char	*value;

	i = 0;
	x = 0;
	c = '+';
	while (str[i] && str[i] != '=')
		i++;
	if (!str[i])
		return (NULL);
	i++;
	j = i;
	while (str[j])
	{
		if (str[j] == '\"' || str[j] == '\'')
		{
			if (str[j] == c)
				c = '+';
			else if(c == '+')
				c = str[j];
			else
				x++;
		}
		else
			x++;
		j++;
	}
	value = malloc(sizeof(char) * (x + 1));
	if (!value)
		exit(1);
	new_improved_strcpy(value, str + i);
	return (value);
}
