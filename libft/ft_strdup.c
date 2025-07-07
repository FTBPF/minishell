/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:54:43 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/07 19:54:51 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *src)
{
	int		count;
	char	*dupe;

	count = ft_strlen(src);
	dupe = malloc(sizeof(char) * (count + 1));
	if (!dupe)
		return (0);
	count = 0;
	while (src[count])
	{
		dupe[count] = src[count];
		count++;
	}
	dupe[count] = '\0';
	return (dupe);
}
