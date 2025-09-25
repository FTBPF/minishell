/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:52 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/25 13:34:52 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static void	print_heredoc_error(void)
{
	ft_putstr_fd("minishell: syntax error near", 2);
	ft_putstr_fd(" unexpected token `newline'\n", 2);
	g_exit_status = 2;
}

static void	cleanup_heredoc(t_vars *vars, int j)
{
	close(vars->here_doc_fd[j]);
	unlink(vars->temp);
	free(vars->temp);
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
			return (print_heredoc_error());
		}
		next = ft_strchr(after, '<');
		if (next && *(next + 1) == '<')
			cleanup_heredoc(vars, *j);
		(*j)++;
		tmp = after;
	}
}
