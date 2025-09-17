/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:52:18 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/17 15:30:13 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_unquoted_char(char *str, char c)
{
	int		in_quotes;
	char	current_quote;
	int		i;

	in_quotes = 0;
	current_quote = '\0';
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!in_quotes)
			{
				in_quotes = 1;
				current_quote = str[i];
			}
			else if (str[i] == current_quote)
			{
				in_quotes = 0;
				current_quote = '\0';
			}
		}
		else if (!in_quotes && str[i] == c)
			return (&str[i]);
		i++;
	}
	return (NULL);
}

int setup_input_redirection(char **commands, t_vars *vars, int *j)
{
    char    *infile;
    char    *temp;
    int     i;
    int     in_quotes;
    char    cur_quote;
    int     fd;

    temp = commands[0];
    vars->fd0 = -1;
    infile = NULL;

    while ((temp = find_unquoted_char(temp, '<')))
    {
        if (*(temp + 1) == '<')
        {
            handle_heredoc(vars, temp, j);
            return (vars->redirection_failed ? 0 : 1);
        }
        temp++;
        while (*temp == ' ' || *temp == '\t')
            temp++;
        if (*temp == '\0')
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
            g_exit_status = 2;
            vars->redirection_failed = true;
            return (0);
        }
        i = 0;
        in_quotes = 0;
        cur_quote = '\0';
        while (temp[i] && ((temp[i] != ' ' && temp[i] != '\t'
                && temp[i] != '<' && temp[i] != '>') || in_quotes))
        {
            if (temp[i] == '\'' || temp[i] == '"')
            {
                if (!in_quotes)
                {
                    in_quotes = 1;
                    cur_quote = temp[i];
                }
                else if (temp[i] == cur_quote)
                {
                    in_quotes = 0;
                    cur_quote = '\0';
                }
            }
            i++;
        }

        if (infile)
            free(infile);
        infile = ft_strndup(temp, i);
        if (!infile || infile[0] == '\0')
        {
            ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2);
            g_exit_status = 2;
            vars->redirection_failed = true;
            free(infile);
            return (0);
        }
        fd = open(remove_quotes_from_string(ft_strdup(infile)), O_RDONLY);
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(infile, 2);
            ft_putstr_fd(": ", 2);
            ft_putendl_fd(strerror(errno), 2);
            g_exit_status = 1;
            vars->redirection_failed = true;
            free(infile);
            return (0);
        }
        if (vars->fd0 != -1)
            close(vars->fd0);
        vars->fd0 = fd;
        temp += i;
    }
    if (infile)
    {
        vars->infile_name = infile;
        handle_file_opening(commands[0], vars, infile, j);
        return (1);
    }
    return (0);
}


void	handle_output_redirection(t_vars *vars, char *outfile)
{
	vars->outfile_name = remove_quotes_from_string(ft_strdup(outfile));
	free(outfile);
	if (vars->fd1 < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(vars->outfile_name, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		g_exit_status = 1;
		vars->redirection_failed = true;
	}
}

int	setup_output_redirection(char **commands, t_vars *vars)
{
	char	*outfile;
	char	*temp;
	int		in_quotes;

	outfile = NULL;
	temp = (commands[0]);
	in_quotes = -1;
	while (temp && *temp != '>')
	{
		if (*temp == '"' || *temp == 39)
			in_quotes *= -1;
		temp++;
	}
	if (in_quotes == 1)
		return (0);
	outfile = setup_output_redirection_help(commands, vars, temp, outfile);
	handle_output_redirection(vars, outfile);
	return (1);
}
