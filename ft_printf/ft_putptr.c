/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 19:16:51 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/21 14:01:57 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_lenptr(unsigned long long n)
{
	int	count;

	count = 0;
	while (n > 0)
	{
		count++;
		n /= 16;
	}
	return (count);
}

static void	ft_putptr(unsigned long long n)
{
	if (n >= 16)
	{
		ft_putptr(n / 16);
		ft_putptr(n % 16);
	}
	else
	{
		if (n <= 9)
			ft_putchar(n + '0');
		else
			ft_putchar((n - 10) + 'a');
	}
}

int	ft_printptr(unsigned long long n)
{
	int	count;

	count = 0;
	if (n == 0)
		count += write(1, "(nil)", 5);
	else
	{
		count += write(1, "0x", 2);
		ft_putptr(n);
		count += ft_lenptr(n);
	}
	return (count);
}
