/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:52 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 12:12:37 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// searches the '<<' and writes the name inside vars->temp
// opens a file to write the output of the terminal
// saves the fd in vars->here_doc_fd
void	open_doc(t_vars *vars, char *commands, int *j)
{
	char	*doc_file;
	int		i;

	i = 0;
	commands = ft_strchr(commands, '<');
	commands += 2;
	while (*commands == ' ' || *commands == '	')
		commands++;
	if (*commands == '\0')
	{
		vars->redirection_failed = true;
		return ;
	}
	ft_open_helper(&i, commands);
	doc_file = ft_strndup_aspas(commands, i);
	vars->temp = doc_file;
	doc_file = ft_strjoin(doc_file, "\n");
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

void	process_heredoc(t_vars *vars, char *doc_file, int fd)
{
	char	*str;

	write(1, "> ", 2);
	str = get_next_line(0);
	while (ft_strncmp(str, doc_file, ft_strlen(str)) != 0)
	{
		write(fd, str, ft_strlen(str));
		free(str);
		write(1, "> ", 2);
		str = get_next_line(0);
	}
	free(str);
	str = NULL;
	get_next_line(-1);
	free(doc_file);
	ft_free_vars(vars);
	g_exit_status = 0;
	exit(g_exit_status);
}

// Searches the commands matrix for '<<'
// and calls open_doc
// If there are more than one '<<' it overwrites the previous ones
void	here_doc(t_vars *vars, char **commands)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (commands[i])
	{
		if (has_unquoted_heredoc(commands[i]))
			j++;
		i++;
	}
	vars->here_doc_fd = malloc(sizeof(char *) * j + 1);
	if (!vars->here_doc_fd)
		return ;
	vars->here_doc_fd[j] = '\0';
	i = 0;
	j = 0;
	while (commands[i])
	{
		tmp = commands[i];
		handle_heredoc(vars, tmp, &j);
		i++;
	}
}

void	handle_heredoc(t_vars *vars, char *tmp, int *j)
{
	char		*after;
	char		*next;
	static int	printed = 0;

	while (has_unquoted_heredoc(tmp))
	{
		after = ft_strchr(tmp, '<');
		if (!after || *(after + 1) != '<')
			break ;
		after += 2;
		while (*after == ' ' || *after == '\t')
			after++;
		open_doc(vars, tmp, j);
		if (vars->redirection_failed && printed == 0)
		{
			printed++;
			ft_putstr_fd("minishell: syntax error near", 2);
			ft_putstr_fd(" unexpected token `newline'\n", 2);
			g_exit_status = 2;
			return ;
		}
		next = ft_strchr(after, '<');
		if (next && *(next + 1) == '<')
		{
			close(vars->here_doc_fd[*j]);
			unlink(vars->temp);
			free(vars->temp);
		}
		(*j)++;
		tmp = after;
	}
}
