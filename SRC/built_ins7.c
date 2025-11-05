/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins7.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 13:20:40 by frteixei          #+#    #+#             */
/*   Updated: 2025/11/05 15:21:27 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ctp_helper(char const *s, char c, char *d, size_t *i)
{
	while (s[*i] && (s[*i] != c || *d != '+'))
	{
		if (s[*i] == '\"' || s[*i] == '\'')
		{
			if (s[*i] == *d)
				*d = '+';
			else if (*d == '+')
				*d = s[*i];
		}
		(*i)++;
	}
}

void	put_matrix_helper(char const **s, char c, char *d, char *str)
{
	int	i;

	i = 0;
	while (**s && (**s != c || *d != '+'))
	{
		if (**s == '\"' || **s == '\'')
		{
			if (**s == *d)
				*d = '+';
			else if (*d == '+')
				*d = **s;
		}
		str[i] = **s;
		i++;
		(*s)++;
	}
	str[i] = '\0';
}

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
	exit(status);
}

void	ft_exit(t_vars *vars, char **split_cmds)
{
	int		i;
	char	*split;

	i = 0;
	while (split_cmds[i])
		i++;
	if (i == 1)
		*exit_status() = EXIT_SUCCESS;
	else
	{
		if (i > 2)
			return (*exit_status() = 1,
				ft_putstr_fd("minishell: exit: too many arguments\n", 2));
		check_if_exit_stat(&split_cmds[1], 0, 0);
		split = remove_quotes_from_string(split_cmds[1]);
		if (!split || !split[0])
			ft_exit_error(": numeric argument required\n", split_cmds[1], 2, vars);
		if (!ft_is_valid_number(split))
			ft_exit_error(": numeric argument required\n", split, 2, vars);
		*exit_status() = ft_atoi(split);
		free(split);
	}
	if (split_cmds)
		ft_free(split_cmds);
	ft_printf("exit\n");
	rl_clear_history();
	if (vars->my_environ)
		ft_free(vars->my_environ);
	exit((unsigned char)*exit_status());
}
