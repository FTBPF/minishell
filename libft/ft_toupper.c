/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:32:52 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 15:42:40 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

/* int	main(void)
{
	char	test_chars[] = {'a', 'b', 'c', 'z', 'A', 'B', '1', ' ', '@', '\n'};
	int		num_chars;
	int		i;
	char	original;
	char	converted;

	num_chars = sizeof(test_chars) / sizeof(test_chars[0]);
	i = 0;
	printf("Testing ft_toupper:\n");
	while (i < num_chars)
	{
		original = test_chars[i];
		converted = ft_toupper(original);
		printf("Original: %c -> Converted: %c\n", original, converted);
		i++;
	}
	return (0);
} */
