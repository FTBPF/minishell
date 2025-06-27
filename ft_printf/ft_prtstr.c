/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prtstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:56:18 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/21 15:48:34 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(int c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putstr(char *str)
{
	int	count;

	count = 0;
	if (str == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (str[count])
	{
		write(1, &str[count], 1);
		count++;
	}
	return (count);
}

static int	prtnum(unsigned int n)
{
	int	count;

	count = 0;
	if (n >= 10)
		count += prtnum(n / 10);
	count += write(1, &"0123456789"[n % 10], 1);
	return (count);
}

int	ft_putnbr(int nb)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (nb < 0)
	{
		count += write(1, "-", 1);
		nb = -nb;
	}
	count += prtnum(nb);
	return (count);
}

int	ft_putunbr(unsigned int nb)
{
	int	count;

	count = 0;
	count += prtnum(nb);
	return (count);
}
