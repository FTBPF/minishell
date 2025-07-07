/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 15:25:45 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 15:49:36 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	count;
	size_t	str_len;
	char	*substr;

	str_len = ft_strlen(s);
	if (!s)
		return (NULL);
	if (start >= str_len)
		len = 0;
	else if (start + len >= str_len)
		len = str_len - start;
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	count = 0;
	while (count < len && s[start + count])
	{
		substr[count] = s[start + count];
		count++;
	}
	substr[count] = '\0';
	return (substr);
}

/* int	main(void)
{
	char	*str;
	char	*result;

	str = "Hello, World!";
	// Test Case 1: Normal substring
	result = ft_substr(str, 7, 5);
	printf("Substring of '%s' from index 7 with length 5: '%s'\n", str, result);
	free(result);
	// Test Case 2: Start index out of bounds
	result = ft_substr(str, 20, 5);
	printf("Substring of '%s' from index 20 with length 5: '%s'\n", str,
		result);
	free(result);
	// Test Case 3: Length goes beyond the end of the string
	result = ft_substr(str, 7, 50);
	printf("Substring of '%s' from index 7 with length 50: '%s'\n", str,
		result);
	free(result);
	// Test Case 4: Start index at the beginning, length 0
	result = ft_substr(str, 0, 0);
	printf("Substring of '%s' from index 0 with length 0: '%s'\n", str, result);
	free(result);
	// Test Case 5: Empty string as input
	result = ft_substr("", 0, 5);
	printf("Substring of an empty string from index 0 with length 5: '%s'\n",
		result);
	free(result);
	return (0);
} */
