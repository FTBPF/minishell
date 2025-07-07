/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:21:16 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 16:38:59 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char		*last_occ;

	last_occ = NULL;
	while (*s)
	{
		if (*s == (unsigned char)c)
			last_occ = s;
		s++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)s);
	return ((char *)last_occ);
}

/* int	main(void)
{
	const char	str[] = "Hello, world!";
	char		ch;
	char		*result;

	ch = 'o';
	result = ft_strrchr(str, ch);
	if (result)
		printf("Last occurrence of character '%c' found at position: %ld\n", ch,
			result - str);
	else
		printf("Character '%c' not found in the string.\n", ch);
	ch = 'z';
	result = ft_strrchr(str, ch);
	if (result)
		printf("Last occurrence of character '%c' found at position: %ld\n", ch,
			result - str);
	else
		printf("Character '%c' not found in the string.\n", ch);
	ch = '\0';
	result = ft_strrchr(str, ch);
	if (result)
		printf("Null terminator found at position: %ld\n", result - str);
	else
		printf("Null terminator not found in the string.\n");
	return (0);
} */
