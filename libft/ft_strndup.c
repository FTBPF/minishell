/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:40:35 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/07 19:52:32 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	size_t len;
	char	*dupe;

	len = ft_strlen(s);
	if (n < len)
		len = n;
	dupe = (char *)malloc(len + 1);
	if (!dupe)
		return (NULL);
	ft_memcpy(dupe, s, len);
	dupe[len] = '\0';
	return (dupe);
}

/* int	main(void)
{
	char	source[] = "Hello, world!";
	char	*duplicate;

	duplicate = ft_strdup(source);
	if (duplicate)
	{
		printf("Original: %s\n", source);
		printf("Duplicate: %s\n", duplicate);
		free(duplicate);
	}
	else
		printf("Memory allocation failed.\n");
	return (0);
} */
