/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:04 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/23 17:12:50 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_open_doc(t_vars *vars, char *doc_file, char *temp_name)
{
	if (!doc_file)
	{
		free(temp_name);
		return (vars->redirection_failed = true, (void)0);
	}
	if (vars->temp)
	{
		unlink(vars->temp);
		free(vars->temp);
	}
}

void	open_doc(t_vars *vars, char *commands, int *j)
{
	char	*doc_file;
	char	*temp_name;
	int		i;

	i = 0;
	commands = ft_strchr(commands, '<');
	commands += 2;
	while (*commands == ' ' || *commands == '	')
		commands++;
	if (*commands == '\0')
		return (vars->redirection_failed = true, (void)0);
	ft_open_helper(&i, commands);
	doc_file = ft_strndup_aspas(commands, i);
	if (!doc_file)
		return (vars->redirection_failed = true, (void)0);
	temp_name = doc_file;
	doc_file = ft_strjoin(temp_name, "\n");
	check_open_doc(vars, doc_file, temp_name);
	vars->temp = temp_name;
	open_doc_file(vars, doc_file, j);
}

void	open_doc_file(t_vars *vars, char *doc_file, int *j)
{
	int	id;

	vars->here_doc_fd[*j] = open(vars->temp, O_CREAT | O_TRUNC | O_RDWR,
			0000644);
	if (vars->here_doc_fd[*j] == -1)
		perror(vars->temp);
	id = fork();
	if (id == 0)
		process_heredoc(vars, doc_file, vars->here_doc_fd[*j]);
	wait(NULL);
	free(doc_file);
	vars->here_doc_fd[*j] = open(vars->temp, O_RDONLY, 0000644);
}

void	ft_open_helper(int *i, char *commands)
{
	char	current_quote;
	int		in_quotes;

	in_quotes = -1;
	current_quote = '\0';
	while (commands[*i])
	{
		if (in_quotes == -1 && (commands[*i] == ' ' || commands[*i] == '<'
				|| commands[*i] == '>'))
			break ;
		if ((commands[*i] == '\'' || commands[*i] == '\"') && (in_quotes == -1
				|| current_quote == commands[*i]))
		{
			in_quotes *= -1;
			if (in_quotes == 1)
				current_quote = commands[*i];
			else
				current_quote = '\0';
		}
		(*i)++;
	}
}

char	*ft_strndup_aspas(char *commands, int len)
{
	int		i;
	char	*new_str;

	i = 0;
	new_str = (char *)malloc(len + 1);
	if (!new_str && !commands)
		return (NULL);
	ft_aspas_helper(len, &i, new_str, commands);
	new_str[i] = '\0';
	return (new_str);
}
