/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:33:06 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/07 19:53:07 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *str)
{
	size_t	count;

	count = 0;
	while (str[count] != '\0')
		count++;
	return (count);
}

/* int	main(void)
{
	char test_str[] = "Hello, world!";
	int length;

	length = ft_strlen(test_str);
	printf("Length of '%s' is %d\n", test_str, length);
	return (0);
} */
