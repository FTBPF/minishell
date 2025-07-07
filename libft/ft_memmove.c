/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:13:39 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 16:56:28 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*from;
	unsigned char		*to;

	from = (const unsigned char *)src;
	to = (unsigned char *)dest;
	if (to == from || n == 0)
		return (dest);
	if (to < from)
	{
		while (n--)
			*to++ = *from++;
	}
	else if (to > from)
	{
		to += n;
		from += n;
		while (n--)
		{
			*(--to) = *(--from);
		}
	}
	return (dest);
}

/* int	main(void)
{
	char	str1[] = "Hello, World!";
	char	str2[20];
	char	overlap_test[] = "123456789";

	ft_memmove(str2, str1, 14);
	printf("Cópia sem sobreposição: %s\n", str2);
	ft_memmove(overlap_test + 4, overlap_test, 5);
	printf("Cópia com sobreposição: %s\n", overlap_test);
	return (0);
} */
