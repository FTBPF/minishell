/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:25:15 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 18:26:38 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	return ((c >= 0 && c <= 127));
}

/* int	main(void)
{
	char test_chars[] = {'A', 'z', 128, 127, '\n', ' ', -1, '!', '\0'};
	int i = 0;

	while (i < sizeof(test_chars) / sizeof(test_chars[0]))
	{
		char c = test_chars[i];
		if (ft_isascii(c))
			printf("'%c' is an ASCII character.\n", c);
		else
			printf("'%c' NOT an ASCII character.\n", c);
		i++;
	}
	return (0);
} */
