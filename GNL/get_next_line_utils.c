/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 16:37:55 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/07 19:46:52 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_linelen(char *str)
{
	int	count;

	count = 0;
	while (str && str[count] && str[count] != '\n')
		count++;
	if (str && str[count] == '\n')
		count++;
	return (count);
}

char	*ft_strjoingnl(char *s1, char *s2)
{
	int		count;
	int		count2;
	char	*joined;

	count = 0;
	joined = (char *)malloc(ft_linelen(s1) + ft_linelen(s2) + 1);
	if (!joined)
		return (free(s1), NULL);
	while (s1 && s1[count])
	{
		joined[count] = s1[count];
		count++;
	}
	count2 = 0;
	while (s2[count2] != '\n' && s2[count2])
	{
		joined[count + count2] = s2[count2];
		count2++;
	}
	if (s2[count2] == '\n')
		joined[count + count2++] = '\n';
	joined[count + count2] = '\0';
	if (s1)
		free(s1);
	return (joined);
}

void	buffer_clean(char *str)
{
	int	count;
	int	count2;

	count = 0;
	count2 = 0;
	while (str[count] != '\n' && str[count] != '\0')
		count++;
	if (str[count] == '\n')
	{
		count++;
		while (str[count])
			str[count2++] = str[count++];
	}
	while (count2 < BUFFER_SIZE)
		str[count2++] = '\0';
}
