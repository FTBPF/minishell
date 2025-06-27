/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 19:18:33 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/21 14:00:45 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_lenhex(unsigned int n)
{
	int	count;

	count = 0;
	while (n)
	{
		count++;
		n /= 16;
	}
	return (count);
}

static int	ft_puthex(unsigned int n, const char base)
{
	int	count;

	count = 0;
	if (n >= 16)
	{
		ft_puthex(n / 16, base);
		ft_puthex(n % 16, base);
	}
	else
	{
		if (n <= 9)
			count += ft_putchar(n + '0');
		else
		{
			if (base == 'x')
				count += ft_putchar((n - 10) + 'a');
			else if (base == 'X')
				count += ft_putchar((n - 10) + 'A');
		}
	}
	return (count);
}

int	ft_printhex(unsigned int n, const char base)
{
	if (n == 0)
	{
		write(1, "0", 1);
		return (1);
	}
	else
		ft_puthex(n, base);
	return (ft_lenhex(n));
}
