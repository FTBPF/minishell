/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:43:18 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/22 12:56:38 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	put_arg(va_list args, const char type)
{
	int	count;

	count = 0;
	if (type == 'c')
		count += ft_putchar(va_arg(args, int));
	else if (type == 's')
		count += ft_putstr(va_arg(args, char *));
	else if (type == 'i' || type == 'd')
		count += ft_putnbr(va_arg(args, int));
	else if (type == '%')
		count += ft_putchar('%');
	else if (type == 'x' || type == 'X')
		count += ft_printhex(va_arg(args, unsigned int), type);
	else if (type == 'u')
		count += ft_putunbr(va_arg(args, unsigned int));
	else if (type == 'p')
		count += ft_printptr(va_arg(args, unsigned long long));
	else
		count += write(1, &type, 1);
	return (count);
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;
	int		count_char;

	va_start(args, str);
	count = 0;
	count_char = 0;
	if (!str)
		return (-1);
	while (str[count])
	{
		if (str[count] == '%')
		{
			while (str[count + 1] == ' ')
				count++;
			count_char += put_arg(args, str[count + 1]);
			count++;
		}
		else
			count_char += ft_putchar(str[count]);
		count++;
	}
	va_end(args);
	return (count_char);
}
