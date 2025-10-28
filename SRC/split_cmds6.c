/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:01:17 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/28 17:02:28 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*expand_exit_status(char *result, int *i)
{
	char	*temp;
	char	*num;

	num = ft_itoa(g_exit_status);
	temp = result;
	result = ft_strjoin(result, num);
	free(temp);
	free(num);
	*i += 2;
	return (result);
}

static char	*expand_env_var(t_vars *vars, char *line, char *result, int *i)
{
	int		j;
	char	*name;
	char	*val;
	char	*temp;

	j = *i + 1;
	while (line[j] && (ft_isalnum(line[j]) || line[j] == '_'))
		j++;
	if (j == *i + 1)
		return (ft_strjoin_char(result, line[(*i)++]));
	name = ft_substr(line, *i + 1, j - *i - 1);
	val = get_env_var(vars, name);
	free(name);
	temp = result;
	if (val)
		result = ft_strjoin(result, val);
	else
		result = ft_strjoin_char(result, '$');
	free(temp);
	*i = j;
	return (result);
}

static char	*append_char(char *result, char c)
{
	char	*temp;

	temp = result;
	result = ft_strjoin_char(result, c);
	free(temp);
	return (result);
}

char	*expand_heredoc_line(t_vars *vars, char *line)
{
	char	*res;
	int		i;

	if (!ft_strchr(line, '$'))
		return (ft_strdup(line));
	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '?')
			res = expand_exit_status(res, &i);
		else if (line[i] == '$' && line[i + 1] && line[i + 1] != ' ' && line[i
				+ 1] != '\t' && line[i + 1] != '\n')
			res = expand_env_var(vars, line, res, &i);
		else
			res = append_char(res, line[i++]);
	}
	return (res);
}
