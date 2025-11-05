/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:21:26 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 16:39:52 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_cd_and_expansion(t_vars *vars, char **cmds, char *input)
{
	if (ft_strchr(input, '$'))
		var_expander(vars, cmds);
	if (check_cd_ex_uns(cmds, vars))
	{
		ft_free_vars(vars);
		return (1);
	}
	return (0);
}

static int	handle_setup_error(char ***cmds, t_vars *vars)
{
	ft_free(*cmds);
	*cmds = NULL;
	vars->all_commands = NULL;
	return (0);
}

static int	handle_heredoc_phase(t_vars *vars, char ***cmds)
{
	here_doc(vars, *cmds);
	if (vars->redirection_failed && *exit_status() == 130)
		return (handle_setup_error(cmds, vars));
	return (1);
}

int	setup_commands(char *input, t_vars *vars, char ***cmds)
{
	if (check_pipe_syntax(input))
		return (0);
	*cmds = ft_split_commands(input, "|");
	if (!*cmds)
		return (0);
	vars->all_commands = *cmds;
	if (!validate_commands(*cmds))
		return (0);
	vars->in_pipeline = ((*cmds)[1] != NULL);
	if (handle_cd_and_expansion(vars, *cmds, input))
		return (handle_setup_error(cmds, vars));
	vars->redirection_failed = false;
	return (handle_heredoc_phase(vars, cmds));
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
