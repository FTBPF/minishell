/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 18:48:41 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/19 17:47:19 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_printf(const char *str, ...);
int	ft_putnbr(int n);
int	ft_putunbr(unsigned int i);
int	ft_putstr(char *str);
int	ft_putchar(int i);
int	ft_printhex(unsigned int i, const char variavel);
int	ft_putunbr(unsigned int i);
int	ft_printptr(unsigned long long i);

#endif