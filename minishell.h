/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: franc <franc@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:11:28 by franc             #+#    #+#             */
/*   Updated: 2025/06/26 16:54:32 by franc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include "GNL/get_next_line.h"
# include "GNL/get_next_line.h"
# include "ft_printf/ft_printf.h"

typedef struct s_vars
{
	int		i;
	int		j;
	int		fd[2];
	int		pipe_fd[2];
	int		pid1;
	int		fd1;
	int		fd0;
	int		p0;
	char 	*temp;
	char 	*cmd1_path;
	char 	*cmd2_path;
	char	**cmd_flags;
	char	**cmd2_flags;
	char	*here_doc_fd;
	char	**my_environ;
	int		exit_status;
	int		num_env_vars;
}	t_vars;

#endif