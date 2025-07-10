/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marada <marada@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 02:41:50 by marada            #+#    #+#             */
/*   Updated: 2025/07/10 16:12:39 by marada           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		while (s[i] && (s[i] != c || d != '+'))
		{
			if (s[i] == '\"' || s[i] == '\'')
			{
				if (s[i] == d)
					d = '+';
				else if(d == '+')
					d = s[i];
			}
			i++;
		}
		ctp++;
		while (s[i] && s[i] == c)
		i++;
	}
	return (ctp);
}

static char	**ft_putmatrix(char **matrix, char const *s, char c, size_t	ctp)
{
	size_t	i;
	char	d;
	size_t	j;
	
	i = 0;
	d = '+';
	j = 0;
	while (*s && *s == c)
	s++;
	while (ctp)
	{
		matrix[j] = (char *)malloc(sizeof(char) * (ft_tamnhoplavra(s, c) + 1));
		i = 0;
		while (*s && (*s != c || d != '+'))
		{
			if (*s == '\"' || *s == '\'')
			{
				if (s[i] == d)
					d = '+';
				else if(d == '+')
					d = s[i];
			}
			matrix[j][i] = *s;
			i++;
			s++;
		}
		matrix[j][i] = '\0';
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
	printf("string:%s\n", s);
	printf("ctp:%li\n", ft_ctp(s, c));
	matrix = (char **)malloc(sizeof(char *) * (ft_ctp(s, c) + 1));
	if (!matrix || !s)
		return (0);
	matrix = ft_putmatrix(matrix, s, c, ft_ctp(s, c));
	return (matrix);	
}