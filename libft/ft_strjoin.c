/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:47:56 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 18:15:40 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	int		count1;
	int		count2;
	char	*joinstr;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	count1 = -1;
	count2 = -1;
	joinstr = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!joinstr)
		return (NULL);
	while (++count1 < s1_len)
		joinstr[count1] = s1[count1];
	while (++count2 < s2_len)
		joinstr[count1 + count2] = s2[count2];
	joinstr[s1_len + s2_len] = '\0';
	return (joinstr);
}

/* int	main(void)
{
	char	str1[] = "Hello, ";
	char	str2[] = "world!";
	char	*result;

	result = ft_strjoin(str1, str2);
	if (result)
	{
		printf("Result: %s\n", result);
		free(result);
	}
	else
		printf("Memory allocation failed.\n");
	return (0);
} */
