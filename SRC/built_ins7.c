/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins7.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:40 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 17:01:23 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	ft_exit_error(char *msg, char *arg, int status, t_vars *vars)
{
	*exit_status() = status;
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(msg, 2);
	if (vars->my_environ)
		ft_free(vars->my_environ);
	if (vars->all_commands)
	{
		ft_free(vars->all_commands);
		vars->all_commands = NULL;
	}
	exit(status);
}

static void	exit_with_error(char *msg, char *arg, int code, t_vars *vars)
{
	ft_exit_error(msg, arg, code, vars);
}

static void	handle_exit_args(t_vars *vars, char **cmds, int count)
{
	char	*split;

	if (count > 2)
	{
		*exit_status() = 1;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return ;
	}
	check_if_exit_stat(&cmds[1], 0, 0);
	split = remove_quotes_from_string(cmds[1]);
	if (!split || !split[0])
		exit_with_error(": numeric argument required\n", cmds[1], 2, vars);
	if (!ft_is_valid_number(split))
		exit_with_error(": numeric argument required\n", split, 2, vars);
	*exit_status() = ft_atoi(split);
	free(split);
}

void	ft_exit(t_vars *vars, char **cmds)
{
	int	count;

	count = 0;
	while (cmds[count])
		count++;
	if (count == 1)
		*exit_status() = EXIT_SUCCESS;
	else
		handle_exit_args(vars, cmds, count);
	if (cmds)
		ft_free(cmds);
	ft_printf("exit\n");
	rl_clear_history();
	if (vars->my_environ)
		ft_free(vars->my_environ);
	if (vars->all_commands)
	{
		ft_free(vars->all_commands);
		vars->all_commands = NULL;
	}
	exit((unsigned char)*exit_status());
}
