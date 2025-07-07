/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:33:54 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 16:10:22 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	unsigned char	*ptr;

	ptr = str;
	while (n > 0)
	{
		*ptr = (unsigned char)c;
		ptr++;
		n--;
	}
	return (str);
}

/* int	main(void)
{
	char	str[50] = "Hello, world!";
	size_t	num_bytes;

	num_bytes = 5;
	printf("Before ft_memset: %s\n", str);
	ft_memset(str, '*', num_bytes);
	printf("After ft_memset: %s\n", str);
	return (0);
} */
