/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:15:14 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 18:27:02 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

/* int	main(void)
{
	char testChars[] = {'A', 'z', '0', '!', ' ', '3', 'g', '@', 'Z', '9', '-',
		'b'};
	int numTests = sizeof(testChars) / sizeof(testChars[0]);
	int count = 0;

	printf("Testing ft_isalpha function:\n");
	while (count < numTests)
	{
		char c = testChars[count];
		if (ft_isalpha(c))
			printf("'%c' is alpha.\n", c);
		else
			printf("'%c' not alpha.\n", c);
		count++;
	}
	return (0);
} */
