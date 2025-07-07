/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 15:27:00 by frteixei          #+#    #+#             */
/*   Updated: 2024/11/12 18:28:31 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// aloca mem e inicializa a 0

void	*ft_calloc(size_t num, size_t size)
{
	size_t	total_size;
	void	*ptr;

	total_size = num * size;
	ptr = malloc(total_size);
	if (!ptr)
		return (NULL);
	if (num == 0 || size == 0)
		return (ptr);
	ft_memset(ptr, 0, total_size);
	return (ptr);
}

/* int	main(void)
{
	int		*array;
	size_t	i;

	size_t num_elements = 5;
	size_t element_size = sizeof(int);
	array = (int *)ft_calloc(num_elements, element_size);
	if (array == NULL)
	{
		printf("Memory allocation failed\n");
		return (0);
	}
	printf("Allocated array values:\n");
	i = 0;
	while (i < num_elements)
	{
		printf("array[%zu] = %d\n", i, array[i]);
		i++;
	}
	free(array);
	return (1);
} */
