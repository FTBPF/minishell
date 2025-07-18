/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 17:52:18 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/18 09:15:54 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	setup_input_redirection(char **commands, t_vars *vars, int *j)
{
	char	*infile;
	char	*temp;
	int		i;
	int		in_quotes;

	temp = commands[0];
	in_quotes = -1;
	while (temp && *temp && *temp != '>')
	{
		if (*temp == '"' || *temp == 39)
			in_quotes *= -1;
		temp++;
	}
	if (in_quotes == 1)
		return (0);
	temp = ft_strrchr(commands[0], '<');
	temp++;
	while (*temp == ' ' || *temp == '	')
		temp++;
	i = 0;
	while (temp[i] != ' ' && temp[i] != '	' && temp[i])
		i++;
	infile = ft_strndup(temp, i);
	handle_file_opening(commands[0], vars, infile, j);
	return (1);
}

void handle_output_redirection(t_vars *vars, char *outfile)
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
		return ;
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
