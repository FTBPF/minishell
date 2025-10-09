/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 02:41:50 by marada            #+#    #+#             */
/*   Updated: 2025/10/09 15:26:11 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Counts characters until the delimiter is found, but ignores delimiters
// inside quotes (single or double). Tracks quote state with a flag.

static size_t	ft_tamnhoplavra(char const *s, char c)
{
	size_t	i;
	int		flag;

	flag = 0;
	i = 0;
	while (s[i] && (s[i] != c || flag == 1))
	{
		if (s[i] == '\"' || s[i] == '\'')
		{
			if (flag == 0)
				flag = 1;
			else
				flag = 0;
		}
		i++;
	}
	return (i);
}

// Counts words separated by the delimiter, respecting quotes.

static size_t	ft_ctp(char const *s, char c)
{
	size_t	i;
	char	d;
	size_t	ctp;

	d = '+';
	i = 0;
	ctp = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		ctp_helper(s, c, &d, &i);
		ctp++;
		while (s[i] && s[i] == c)
			i++;
	}
	return (ctp);
}

// Allocates memory for each word and copies it into the matrix,
// respecting quotes. Uses put_matrix_helper for the actual copying.

static char	**ft_putmatrix(char **matrix, char const *s, char c, size_t ctp)
{
	char	d;
	size_t	j;

	d = '+';
	j = 0;
	while (*s && *s == c)
		s++;
	while (ctp)
	{
		matrix[j] = (char *)malloc(sizeof(char) * (ft_tamnhoplavra(s, c) + 1));
		put_matrix_helper(&s, c, &d, matrix[j]);
		while (*s && *s == c)
			s++;
		j++;
		ctp--;
	}
	matrix[j] = 0;
	return (matrix);
}

char	**ft_split_novo_e_melhorado(char const *s, char c)
{
	char	**matrix;

	if (!s)
		return (0);
	matrix = (char **)malloc(sizeof(char *) * (ft_ctp(s, c) + 1));
	if (!matrix || !s)
		return (0);
	matrix = ft_putmatrix(matrix, s, c, ft_ctp(s, c));
	return (matrix);
}
