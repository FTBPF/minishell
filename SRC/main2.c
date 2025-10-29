/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:26 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/28 16:03:03 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_pipe_syntax(char *input)
{
	int	i;
	int	len;

	if (str_is_spaces_only(input))
		return (1);
	if (input[0] == '|')
	{
		ft_printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	len = ft_strlen(input);
	i = len - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t'))
		i--;
	if (i >= 0 && input[i] == '|')
	{
		ft_printf("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	return (0);
}

static int	validate_commands(char **cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		if (str_is_spaces_only(cmds[i]))
		{
			ft_printf("minishell: syntax error near unexpected token `|'\n");
			ft_free(cmds);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	handle_cd_and_expansion(t_vars *vars, char **cmds, char *input)
{
	if (ft_strchr(input, '$'))
		var_expander(vars, cmds);
	if (check_cd_ex_uns(cmds, vars))
	{
		ft_free_vars(vars);
		ft_free(cmds);
		return (1);
	}
	return (0);
}

int setup_commands(char *input, t_vars *vars, char ***commands)
{
    int invalid;

    invalid = check_pipe_syntax(input);
    if (invalid)
        return (0);
    *commands = ft_split_commands(input, "|");
    if (!*commands)
        return (0);
    
    // ADD THIS HERE - so heredoc child can free it
    vars->all_commands = *commands;
    
    if (!validate_commands(*commands))
        return (0);
    vars->in_pipeline = ((*commands)[1] != NULL);
    if (handle_cd_and_expansion(vars, *commands, input))
        return (0);
    vars->redirection_failed = false;
    here_doc(vars, *commands);  // Now heredoc child has all_commands set
    if (vars->redirection_failed && g_exit_status == 130)
    {
        ft_free(*commands);
        *commands = NULL;
        vars->all_commands = NULL;  // Clear it
        return (0);
    }
    return (1);
}

int	ft_has_invalid_pipe(char **commands)
{
	int	i;
	int	j;

	if (!commands || !commands[0])
		return (1);
	if (commands[0][0] == '|')
		return (1);
	i = 0;
	while (commands[i])
	{
		j = 0;
		while (commands[i][j])
		{
			if (commands[i][j] != ' ' && commands[i][j] != '\t'
				&& commands[i][j] != '|')
				break ;
			j++;
		}
		if (commands[i][j] == '\0')
			return (1);
		i++;
	}
	return (0);
}
