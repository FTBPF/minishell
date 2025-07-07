/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 13:01:16 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 18:29:59 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// poe "n" bytes da mem pra 0

void	ft_bzero(void *str, size_t n)
{
	unsigned char	*ptr;

	ptr = (unsigned char *)str;
	while (n > 0)
	{
		*ptr++ = 0;
		n--;
	}
}

/* int	main(void)
{
	char buffer[26];
	int i = 0;
	while (i < sizeof(buffer))
	{
		buffer[i] = 'A' + (i % 26);
		i++;
	}
	printf("Buffer before ft_bzero: ");
	i = 0;
	while (i < sizeof(buffer))
	{
		printf("%c ", buffer[i]);
		i++;
	}
	printf("\n");
	ft_bzero(buffer, 10);
	printf("Buffer after ft_bzero: ");
	i = 0;
	while (i < sizeof(buffer))
	{
		printf("%c ", buffer[i]);
		i++;
	}
	printf("\n");
	return (0);
} */
