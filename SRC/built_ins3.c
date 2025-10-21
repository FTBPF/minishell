/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:41:31 by frteixei          #+#    #+#             */
/*   Updated: 2025/10/21 11:10:46 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    ft_env(t_vars *vars)
{
    int i;

    i = 0;
    while (vars->my_environ[i])
    {
        if (ft_strchr(vars->my_environ[i], '='))
            ft_printf("%s\n", vars->my_environ[i]);
        i++;
    }
    g_exit_status = 0;
}
