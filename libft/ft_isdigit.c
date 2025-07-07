/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:21:49 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 18:25:21 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}

/* int	main(void)
{
	char testChars[] = {'A', 'z', '0', '!', ' ', '3', 'g', '@', 'Z', '9', '-',
		'b'};
	int numTests = sizeof(testChars) / sizeof(testChars[0]);
	int count = 0;

	printf("Testing ft_isdigit function:\n");
	while (count < numTests)
	{
		char c = testChars[count];
		if (ft_isdigit(c))
			printf("'%c' is digit.\n", c);
		else
			printf("'%c' not digit.\n", c);
		count++;
	}
	return (0);
} */
