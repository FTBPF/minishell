/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:06:55 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/14 19:22:19 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*ptrdest;
	const unsigned char	*ptrsrc;

	ptrdest = (unsigned char *)dest;
	ptrsrc = (const unsigned char *)src;
	while (n > 0)
	{
		*ptrdest = *ptrsrc;
		ptrdest++;
		ptrsrc++;
		n--;
	}
	return (dest);
}

/* int	main(void)
{
	char src[50] = "This is the source string.";
	char dest[50];

	printf("Before ft_memcpy:\n");
	printf("Source: %s\n", src);
	printf("Destination: %s\n", dest);
	ft_memcpy(dest, src, ft_strlen(src) + 1);
	printf("After ft_memcpy:\n");
	printf("Source: %s\n", src);
	printf("Destination: %s\n", dest);
	return (0);
} */
