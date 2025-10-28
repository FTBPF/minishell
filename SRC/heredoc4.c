/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:56:58 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/28 16:57:53 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*generate_temp_filename(void)
{
	static int	count = 0;
	char		*num_str;
	char		*temp;
	char		*result;

	num_str = ft_itoa(count++);
	temp = ft_strjoin("/tmp/.heredoc_", num_str);
	free(num_str);
	result = ft_strjoin(temp, "_tmp");
	free(temp);
	return (result);
}

static int	delimiter_has_quotes(char *commands)
{
	int	i;

	i = 0;
	while (commands[i] && commands[i] != ' ' && commands[i] != '\t'
		&& commands[i] != '<' && commands[i] != '>')
	{
		if (commands[i] == '\'' || commands[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

static char	*get_delimiter(t_vars *vars, char *commands, int *should_expand)
{
	int		i;
	char	*delimiter;

	i = 0;
	*should_expand = !delimiter_has_quotes(commands);
	ft_open_helper(&i, commands);
	delimiter = ft_strndup_aspas(commands, i);
	if (!delimiter)
		vars->redirection_failed = true;
	return (delimiter);
}

static char	*create_doc_file(t_vars *vars, char *delimiter)
{
	char	*doc_file;

	doc_file = ft_strjoin(delimiter, "\n");
	free(delimiter);
	if (!doc_file)
		vars->redirection_failed = true;
	return (doc_file);
}

void	open_doc(t_vars *vars, char *commands, int *j)
{
	char	*doc_file;
	char	*delimiter;
	char	*temp_name;
	int		should_expand;

	commands = ft_strchr(commands, '<');
	if (!commands || *(commands + 2) == '\0')
		return ((void)(vars->redirection_failed = true));
	commands += 2;
	while (*commands == ' ' || *commands == '\t')
		commands++;
	delimiter = get_delimiter(vars, commands, &should_expand);
	if (vars->redirection_failed)
		return ;
	doc_file = create_doc_file(vars, delimiter);
	if (vars->redirection_failed)
		return ;
	temp_name = generate_temp_filename();
	check_open_doc(vars, doc_file, temp_name);
	vars->temp = temp_name;
	open_doc_file_expanded(vars, doc_file, j, should_expand);
}
