/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 14:55:23 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/14 19:25:36 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	count;

	result = 0;
	sign = 1;
	count = 0;
	while (str[count] == ' ' || (str[count] >= 9 && str[count] <= 13))
		count++;
	if (str[count] == '+' || str[count] == '-')
	{
		if (str[count] == '-')
			sign = -1;
		count++;
	}
	while (str[count] >= '0' && str[count] <= '9')
	{
		result = result * 10 + (str[count] - '0');
		count++;
	}
	return (result * sign);
}

/* int	main(void)
{
	char	*test1;
	char	*test2;
	char	*test3;
	char	*test4;
	char	*test5;
	char	*test6;
	char	*test7;

	test1 = "42";
	test2 = "-42";
	test3 = "   42";
	test4 = "+42";
	test5 = "123abc";
	test6 = "-123abc456";
	test7 = "0";
	char *test8 = "2147483647";
	char *test9 = "-2147483648";
	char *test10 = "9999999999"; //Overflow
	printf("Input: '%s' -> Output: %d\n", test1, ft_atoi(test1));
	printf("Input: '%s' -> Output: %d\n", test2, ft_atoi(test2));
	printf("Input: '%s' -> Output: %d\n", test3, ft_atoi(test3));
	printf("Input: '%s' -> Output: %d\n", test4, ft_atoi(test4));
	printf("Input: '%s' -> Output: %d\n", test5, ft_atoi(test5));
	printf("Input: '%s' -> Output: %d\n", test6, ft_atoi(test6));
	printf("Input: '%s' -> Output: %d\n", test7, ft_atoi(test7));
	printf("Input: '%s' -> Output: %d\n", test8, ft_atoi(test8));
	printf("Input: '%s' -> Output: %d\n", test9, ft_atoi(test9));
	printf("Input: '%s' -> Output: %d\n", test10, ft_atoi(test10));
	return (0);
} */
