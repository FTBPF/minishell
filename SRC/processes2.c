/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:52:18 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/17 18:35:01 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *find_unquoted_char(char *str, char c)
{
    int in_quotes = 0;
    char quote_char = '\0';
    
    while (*str)
    {
        if (*str == '\'' || *str == '"')
        {
            if (!in_quotes)
            {
                in_quotes = 1;
                quote_char = *str;
            }
            else if (*str == quote_char)
            {
                in_quotes = 0;
                quote_char = '\0';
            }
        }
        else if (!in_quotes && *str == c)
        {
            return (str);
        }
        str++;
    }
    return (NULL);
}

int setup_input_redirection(char **commands, t_vars *vars, int *j)
{
    char *infile;
    char *temp;
    int i;
    int in_quotes;
    char cur_quote;
    int fd;
    
    temp = commands[0];
    vars->fd0 = -1;
    infile = NULL;
    while ((temp = find_unquoted_char(temp, '<')))
    {
        if (*(temp + 1) == '<')
        {
            handle_heredoc(vars, temp, j);
            if (vars->redirection_failed)
                return (0);
            temp += 2;
            continue;
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
        if (vars->fd0 != -1)
            close(vars->fd0);
        char *cleaned_filename = remove_quotes_from_string(ft_strdup(infile));
        fd = open(cleaned_filename, O_RDONLY);
        
        if (fd == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cleaned_filename, 2);
            ft_putstr_fd(": ", 2);
            ft_putendl_fd(strerror(errno), 2);
            g_exit_status = 1;
            vars->redirection_failed = true;
            free(cleaned_filename);
            free(infile);
            return (0);
        }
        
        vars->fd0 = fd;
        vars->infile_name = cleaned_filename;
        temp += i;
    }
    
    if (infile)
    {
        free(infile);
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

static char *find_next_output_redirect(char *str)
{
    int in_quotes = 0;
    char quote_char = '\0';
    
    while (*str)
    {
        if (*str == '\'' || *str == '"')
        {
            if (!in_quotes)
            {
                in_quotes = 1;
                quote_char = *str;
            }
            else if (*str == quote_char)
            {
                in_quotes = 0;
                quote_char = '\0';
            }
        }
        else if (!in_quotes && *str == '>')
        {
            return (str);
        }
        str++;
    }
    return (NULL);
}

int setup_output_redirection(char **commands, t_vars *vars)
{
    char *temp;
    char *outfile;
    int i;
    int in_quotes;
    char quote_char;
    bool is_append;
    int last_valid_fd = -1;
    
    temp = commands[0];
    vars->fd1 = -1;
    vars->redirection_failed = false;
    while ((temp = find_next_output_redirect(temp)) != NULL)
    {
        is_append = (*temp == '>' && *(temp + 1) == '>');
        if (is_append)
            temp += 2;
        else
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
        quote_char = '\0';
        
        while (temp[i] && ((temp[i] != ' ' && temp[i] != '\t' 
                && temp[i] != '<' && temp[i] != '>') || in_quotes))
        {
            if (temp[i] == '\'' || temp[i] == '"')
            {
                if (!in_quotes)
                {
                    in_quotes = 1;
                    quote_char = temp[i];
                }
                else if (temp[i] == quote_char)
                {
                    in_quotes = 0;
                    quote_char = '\0';
                }
            }
            i++;
        }
        
        outfile = ft_strndup(temp, i);
        outfile = remove_quotes_from_string(outfile);
        if (vars->fd1 != -1)
            close(vars->fd1);
        if (is_append)
            vars->fd1 = open(outfile, O_CREAT | O_RDWR | O_APPEND, 0644);
        else
            vars->fd1 = open(outfile, O_TRUNC | O_CREAT | O_RDWR, 0644);
            
        if (vars->fd1 == -1)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(outfile, 2);
            ft_putstr_fd(": ", 2);
            ft_putendl_fd(strerror(errno), 2);
            g_exit_status = 1;
            vars->redirection_failed = true;
            free(outfile);
            return (0);
        }
        
        last_valid_fd = vars->fd1;
        vars->outfile_name = outfile;
        temp += i;
    }
    return (last_valid_fd != -1 ? 1 : 0);
}
