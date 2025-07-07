/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 12:57:59 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 18:24:15 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	while (*s)
	{
		if ((unsigned char)*s == uc)
			return ((char *)s);
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return (NULL);
}

/* int	main(void)
{
	const char	*str = "Hello, World!";
	char		*result;

	result = ft_strchr(str, 'o');
	if (result)
		printf("Found 'o' at position: %ld\n", result - str);
	else
		printf("'o' not found in the string.\n");
	result = ft_strchr(str, 'z');
	if (result)
		printf("Found 'z' at position: %ld\n", result - str);
	else
		printf("'z' not found in the string.\n");
	result = ft_strchr(str, '\0');
	if (result)
		printf("Found null terminator at position: %ld\n", result - str);
	else
		printf("Null terminator not found in the string.\n");
	return (0);
} */
