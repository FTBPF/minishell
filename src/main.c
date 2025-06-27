/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franc <franc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:24:05 by franc             #+#    #+#             */
/*   Updated: 2025/06/26 15:26:22 by franc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
	t_vars vars;
	
	(void)argc;
	(void)argv;
	setup_shell(&vars, env);
    run_shell(&vars, env);
	return (0);
}
