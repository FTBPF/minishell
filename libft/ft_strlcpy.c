/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:26:51 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 17:53:40 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_strlcpy(char *dest, const char *src, unsigned int size)
{
	unsigned int	src_len;
	unsigned int	count;

	src_len = ft_strlen(src);
	count = 0;
	if (size > 0)
	{
		while (count < size - 1 && src[count] != '\0')
		{
			dest[count] = src[count];
			count++;
		}
		dest[count] = '\0';
	}
	return (src_len);
}

/* int	main(void)
{
	char src[] = "Hello, world!";
	char dest[20];
	unsigned int size = sizeof(dest);
	unsigned int result;

	result = ft_strlcpy(dest, src, size);
	printf("Source length: %u\n", result);
	printf("Destination string: %s\n", dest);
	return (0);
} */
