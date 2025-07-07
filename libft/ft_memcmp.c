/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 16:29:54 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 17:01:31 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;

	str1 = (const unsigned char *)s1;
	str2 = (const unsigned char *)s2;
	while (n > 0)
	{
		if (*str1 != *str2)
			return ((int)(*str1 - *str2));
		str1++;
		str2++;
		n--;
	}
	return (0);
}

/* int	main(void)
{
	const char str1[] = "abcdef";
	const char str2[] = "abcdeg";
	int result;
	size_t num_bytes = strlen(str1);

	result = ft_memcmp(str1, str2, num_bytes);
	if (result < 0)
		printf("str1 is less than str2\n");
	else if (result > 0)
		printf("str1 is greater than str2\n");
	else
		printf("str1 is equal to str2\n");
	return (0);
} */
