/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 18:51:07 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/11 18:17:33 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	count;

	if (!s || !f)
		return ;
	count = 0;
	while (s[count] != '\0')
	{
		f(count, &s[count]);
		count++;
	}
}

/* void	to_upper(unsigned int index, char *c)
{
	(void)index; // Ignore index if not needed
	if (*c >= 'a' && *c <= 'z')
		*c -= 32; // Convert lowercase to uppercase
}

int	main(void)
{
	char	s[] = "hello world!";

	ft_striteri(s, to_upper);
	printf("Modified string: %s\n", s); // Expected output: "HELLO WORLD!"
	return (0);
} */
