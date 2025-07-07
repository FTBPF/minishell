/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 11:54:42 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/13 15:33:15 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	start;
	size_t	end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	len = end - start;
	if (len == 0)
		return (ft_strdup(""));
	return (ft_substr(s1, start, len));
}

/* int	main(void)
{
	char	str[] = "aaBaBaaThis is a sample string.aBBaaaa";
	char	*trimmed_str;

	printf("Before: %s\n", str);
	trimmed_str = ft_strtrim(str, "aB");
	if (trimmed_str)
	{
		printf("After: %s\n", trimmed_str);
		free(trimmed_str);
	}
	else
		printf("Memory allocation failed\n");
	return (0);
} */
