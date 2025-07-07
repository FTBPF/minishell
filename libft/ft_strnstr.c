/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:10:07 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 16:47:04 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	little_len;

	little_len = ft_strlen(little);
	if (little_len == 0)
		return ((char *)big);
	while (*big && len)
	{
		if (len < little_len)
			return (NULL);
		if (ft_strncmp(big, little, little_len) == 0)
			return ((char *)big);
		big++;
		len--;
	}
	return (NULL);
}

/* int	main(void)
{
	const char	big[] = "Hello, world! Welcome to the world of C programming.";
	const char	little[] = "world";
	char		*result;
	size_t		len;
	const char	little_not_found[] = "programming";

	len = 25;
	result = ft_strnstr(big, little, len);
	if (result)
		printf("Substring '%s' found at position: %ld\n", little, result - big);
	else
		printf("Substring '%s' not found within the first %zu characters.\n",
			little, len);
	result = ft_strnstr(big, little_not_found, len);
	if (result)
		printf("Substring '%s' found at position: %ld\n", little_not_found,
			result - big);
	else
		printf("Substring '%s' not found within the first %zu characters.\n",
			little_not_found, len);
	return (0);
} */
