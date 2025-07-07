/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:44:22 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 17:05:21 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	unsigned char	*haystack;
	unsigned char	needle;

	haystack = (unsigned char *)str;
	needle = (unsigned char)c;
	while (n > 0)
	{
		if (*haystack == needle)
			return ((void *)haystack);
		haystack++;
		n--;
	}
	return (NULL);
}

/* int	main(void)
{
	const char	str[] = "Hello, world!";
	char		*result;
	int			search_char;
	size_t		num_bytes;

	search_char = 'w';
	num_bytes = ft_strlen(str);
	result = ft_memchr(str, search_char, num_bytes);
	if (result)
		printf("Character '%c' found at position: %ld\n", search_char, result
			- str);
	else
		printf("Character '%c' not found within the first %zu bytes\n",
			search_char, num_bytes);
	return (0);
} */
