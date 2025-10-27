/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:04 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/27 17:48:05 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		close(STDIN_FILENO);
		get_next_line(-1);
		exit(130);
	}
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_heredoc_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

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

void	open_doc(t_vars *vars, char *commands, int *j)
{
	char	*doc_file;
	char	*delimiter;
	char	*temp_name;
	int		i;
	int		should_expand;

	i = 0;
	commands = ft_strchr(commands, '<');
	if (!commands)
	{
		vars->redirection_failed = true;
		return ;
	}
	commands += 2;
	while (*commands == ' ' || *commands == '\t')
		commands++;
	if (*commands == '\0')
	{
		vars->redirection_failed = true;
		return ;
	}
	should_expand = !delimiter_has_quotes(commands);
	ft_open_helper(&i, commands);
	delimiter = ft_strndup_aspas(commands, i);
	if (!delimiter)
	{
		vars->redirection_failed = true;
		return ;
	}
	doc_file = ft_strjoin(delimiter, "\n");
	free(delimiter);
	if (!doc_file)
	{
		vars->redirection_failed = true;
		return ;
	}
	temp_name = generate_temp_filename();
	check_open_doc(vars, doc_file, temp_name);
	vars->temp = temp_name;
	open_doc_file_expanded(vars, doc_file, j, should_expand);
}

void	open_doc_file_expanded(t_vars *vars, char *doc_file, int *j,
		int should_expand)
{
	int	id;
	int	write_fd;

	write_fd = open(vars->temp, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (write_fd == -1)
	{
		perror(vars->temp);
		vars->redirection_failed = true;
		free(doc_file);
		return ;
	}
	/* Parent: ignore signals while waiting for heredoc */
	setup_heredoc_parent_signals();
	id = fork();
	if (id == -1)
	{
		perror("fork");
		close(write_fd);
		free(doc_file);
		vars->redirection_failed = true;
		return ;
	}
	if (id == 0)
	{
		/* Child: setup heredoc signal handlers */
		setup_heredoc_signals();
		process_heredoc_expanded(vars, doc_file, write_fd, should_expand);
	}
	close(write_fd);
	wait(NULL);
	free(doc_file);
	vars->here_doc_fd[*j] = open(vars->temp, O_RDONLY);
	if (vars->here_doc_fd[*j] == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(vars->temp);
		vars->redirection_failed = true;
	}
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
