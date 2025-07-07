/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:30:10 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 15:44:00 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

/* int	main(void)
{
	char	test_chars[] = {'A', 'B', 'C', 'Z', 'a', 'b', '1', ' ', '@', '\n'};
	int		num_chars;
	int		i;
	char	original;
	char	converted;

	num_chars = sizeof(test_chars) / sizeof(test_chars[0]);
	i = 0;
	printf("Testing ft_tolower:\n");
	while (i < num_chars)
	{
		original = test_chars[i];
		converted = ft_tolower(original);
		printf("Original: %c -> Converted: %c\n", original, converted);
		i++;
	}
	return (0);
} */
