/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:23:06 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 18:27:26 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0'
			&& c <= '9'));
}

/* int	main(void)
{
	char testChars[] = {'A', 'z', '0', '!', ' ', '3', 'g', '@', 'Z', '9', '-',
		'b'};
	int numTests = sizeof(testChars) / sizeof(testChars[0]);
	int count = 0;

	printf("Testing ft_isalnum function:\n");
	while (count < numTests)
	{
		char c = testChars[count];
		if (ft_isalnum(c))
			printf("'%c' is alphanumeric.\n", c);
		else
			printf("'%c' not alphanumeric.\n", c);
		count++;
	}
	return (0);
} */
