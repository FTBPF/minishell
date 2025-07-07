/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:32:33 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 18:24:55 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	return ((c >= 32 && c <= 126));
}

/* int	main(void)
{
	char test_chars[] = {'a', '1', ' ', '\b', '\n', 126, 127};
	int i;
	
	i = 0;                                              
	printf("Testing ft_isprint function:\n");
	while (i < sizeof(test_chars) / sizeof(test_chars[0]))
	{
		char c = test_chars[i];
		if (ft_isprint(c))
			printf("'%c' is printable.\n", c);
		else
			printf("'%c' is not printable (ASCII: %d).\n", c, c);
		i++;
	}
	return (0);
} */
