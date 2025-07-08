/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 14:50:08 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/08 15:59:23 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countsubstr(char const *s, char c)
{
	size_t	count;

	if (!*s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

static void	free_strings(char **strings, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(strings[i]);
		i++;
	}
	free(strings);
}

static int	extract_word(char **strings, const char **s, char c, size_t *pos)
{
	const char	*word_start;

	word_start = *s;
	while (**s != c && **s)
		(*s)++;
	strings[*pos] = ft_substr(word_start, 0, *s - word_start);
	if (!strings[*pos])
	{
		free_strings(strings, *pos);
		return (0);
	}
	(*pos)++;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	size_t	count;

	strings = (char **)malloc(sizeof(char *) * (ft_countsubstr(s, c) + 1));
	if (!s || !strings)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			if (!extract_word(strings, &s, c, &count))
				return (NULL);
	}
	strings[count] = NULL;
	return (strings);
}
