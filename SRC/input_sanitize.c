/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_sanitize.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:05 by frteixei          #+#    #+#             */
/*   Updated: 2025/07/18 09:18:31 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Checks if the command received in ARGV is valid by
// searching for it in the bin folder

char	*check_executable(char *command, char **split_paths)
{
	if (access(command, X_OK) == 0 || access(command, F_OK) == 0)
	{
		ft_free(split_paths);
		return (command);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		ft_free(split_paths);
		return (NULL);
	}
}

char	*check_command(char *command, char **split_paths)
{
	int		i;
	char	*path;
	char	*temp;

	i = 0;
	while (split_paths[i])
	{
		temp = ft_strjoin(split_paths[i], "/");
		path = ft_strjoin(temp, command);
		free(temp);
		if (access(path, 0) == 0)
		{
			ft_free(split_paths);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free(split_paths);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found\n", 2);
	return (NULL);
}

char	*check_valid_cmd_builtin(char *command)
{
	char	*builtins[8];
	char	*cmd_name;
	int		i;

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	i = 0;
	cmd_name = ft_strrchr(command, '/');
	if (cmd_name)
		command = cmd_name + 1;
	while (builtins[i])
	{
		if (ft_strcmp(command, builtins[i]) == 0)
			return (ft_strdup(command));
		i++;
	}
	return (NULL);
}

char	*check_valid_cmd(char *command, char **envp)
{
	char	*path_var;
	char	**split_paths;
	char	*valid_cmd;

	valid_cmd = check_valid_cmd_builtin(command);
	if (valid_cmd)
		return (valid_cmd);
	if (ft_strchr(command, '/'))
		return (check_executable(command, NULL));
	path_var = find_path(envp);
	if (!path_var)
	{
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": command not found\n", 2);
		return (NULL);
	}
	split_paths = ft_split(path_var, ':');
	valid_cmd = check_command(command, split_paths);
	return (valid_cmd);
}

int	check_flag_n(char *str)
{
	int	f;
	int	flag;

	f = 2;
	flag = 1;
	if (ft_strcmp(str, "-n") == 0)
		return (1);
	else if (ft_strlen(str) > 2 && ft_strncmp(str, "-n", 2) == 0)
	{
		while (str[f])
		{
			if (str[f] != 'n')
				flag = 0;
			f++;
		}
		if (flag == 1)
			return (1);
	}
	return (0);
}
