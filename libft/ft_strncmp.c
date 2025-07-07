/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:27:29 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 17:24:48 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, unsigned int n)
{
	unsigned int	count;

	count = 0;
	while (count < n && s1[count] && s2[count] && (s1[count] == s2[count]))
		count++;
	if (count == n)
		return (0);
	return ((unsigned char)s1[count] - (unsigned char)s2[count]);
}

/* int	main(void)
{
	printf("Test 1: %d\n", ft_strncmp("apple", "apple", 5));
	printf("Test 2: %d\n", ft_strncmp("apple", "applepie", 5));
	printf("Test 3: %d\n", ft_strncmp("apple", "applz", 5));
	printf("Test 4: %d\n", ft_strncmp("apple", "applz", 3));
	printf("Test 5: %d\n", ft_strncmp("apple", "appl", 5));
	printf("Test 6: %d\n", ft_strncmp("", "", 5));
	printf("Test 7: %d\n", ft_strncmp("", "apple", 5));
	printf("Test 8: %d\n", ft_strncmp("apple", "", 5));
	printf("Test 9: %d\n", ft_strncmp("apple", "applepie", 10));
	return (0);
} */
