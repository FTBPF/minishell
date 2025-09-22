/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frteixei <frteixei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 19:42:20 by frteixei          #+#    #+#             */
/*   Updated: 2025/09/22 16:36:07 by frteixei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_file_opening(char *str, t_vars *vars, char *infile, int *j)
{
    (void) infile;
	if (*(ft_strrchr(str, '<') - 1) == '<')
    {
        vars->fd0 = vars->here_doc_fd[*j];
        (*j)++;
    }
}

/* static void	handle_io(t_vars *vars, char **commands)
{
	if (vars->fd1 != 1)
	{
		dup2(vars->fd1, STDOUT_FILENO);
		close(vars->fd1);
	}
	else if (commands[1])
	{
		dup2(vars->pipe_fd[1], STDOUT_FILENO);
		close(vars->pipe_fd[1]);
	}
	close(vars->pipe_fd[0]);
	if (vars->fd0 != 0)
	{
		if (vars->fd0 == -1)
			exit(g_exit_status);
		dup2(vars->fd0, STDIN_FILENO);
		close(vars->fd0);
	}
	else if (vars->p0 != 0)
	{
		dup2(vars->p0, STDIN_FILENO);
		close(vars->p0);
	}
} */

/* static void setup_signals_child(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
} */

// Add this to your execute_command function at the beginning:
void execute_command(t_vars *vars, char **commands, char **envp)
{
    struct stat info;
    
    if (vars->redirection_failed)
        exit(1);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    remove_quotes_from_array(vars->cmd_flags);
    if (check_if_builtin(vars))
	{
        run_builtin(vars);
        exit(g_exit_status);
    }
    vars->cmd1_path = check_valid_cmd(vars->cmd_flags[0], vars->my_environ);
    if (vars->cmd1_path == NULL)
	{
        g_exit_status = 127;
        exit(g_exit_status);
    }
    if (execve(vars->cmd1_path, vars->cmd_flags, envp) == -1)
	{
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(commands[0], 2);
        if (errno == EACCES)
		{
            if (stat(vars->cmd1_path, &info) == 0 && S_ISDIR(info.st_mode))
                ft_putstr_fd(": Is a directory\n", 2);
            else
                ft_putstr_fd(": Permission denied\n", 2);
            g_exit_status = 126;
        }
		else if (errno == ENOENT)
		{
            ft_putstr_fd(": No such file or directory\n", 2);
            g_exit_status = 127;
        }
		else
		{
            ft_putstr_fd(": Execution failed\n", 2);
            g_exit_status = 1;
        }
        exit(g_exit_status);
    }
}

static void	cleanup_temp_file(t_vars *vars)
{
	if (vars->temp != NULL)
	{
		unlink(vars->temp);
		free(vars->temp);
		vars->temp = NULL;
	}
	if (vars->cmd_flags)
		ft_free(vars->cmd_flags);
	vars->cmd_flags = NULL;
}

/* static void close_pipe_fds(int *pipe_fd)
{
    if (pipe_fd[0] != -1) {
        close(pipe_fd[0]);
        pipe_fd[0] = -1;
    }
    if (pipe_fd[1] != -1) {
        close(pipe_fd[1]);
        pipe_fd[1] = -1;
    }
} */

/* static int setup_pipes_for_command(t_vars *vars, char **commands, int cmd_index)
{
    // If not the last command, create a new pipe
    if (commands[cmd_index + 1]) {
        if (pipe(vars->pipe_fd) < 0) {
            perror("pipe");
            return (0);
        }
    } else {
        vars->pipe_fd[0] = -1;
        vars->pipe_fd[1] = -1;
    }
    return (1);
} */

/* static void handle_pipe_io(t_vars *vars, char **commands, int cmd_index, int prev_pipe_read)
{
    // Handle input from previous pipe
    if (cmd_index > 0 && prev_pipe_read != -1) {
        dup2(prev_pipe_read, STDIN_FILENO);
        close(prev_pipe_read);
    } else if (vars->fd0 != 0) {
        dup2(vars->fd0, STDIN_FILENO);
        close(vars->fd0);
    }
    
    // Handle output to next pipe or file
    if (commands[cmd_index + 1]) {
        dup2(vars->pipe_fd[1], STDOUT_FILENO);
        close(vars->pipe_fd[1]);
        close(vars->pipe_fd[0]); // Close read end in child
    } else if (vars->fd1 != 1) {
        dup2(vars->fd1, STDOUT_FILENO);
        close(vars->fd1);
    }
} */

// FIXED FIRST_PROCESS FUNCTION - Replace your current one
void first_process(t_vars *vars, char **envp, char **commands, int *j)
{
    int prev_read_fd; 

	prev_read_fd = vars->p0;
    vars->fd0 = 0;
    vars->fd1 = 1;
    vars->cmd_flags = ft_split_commands_no_redirection(commands[0], " |<>");
    setup_redirections(commands, vars, j);
    if (commands[1]) {
        if (pipe(vars->pipe_fd) < 0) {
            g_exit_status = 1;
            perror("pipe");
            return;
        }
    }
    vars->pid1 = fork();
    if (vars->pid1 < 0) {
        perror("fork");
        return;
    }
    if (vars->pid1 == 0) {
        signal(SIGQUIT, SIG_DFL);
        signal(SIGINT, SIG_DFL);
        if (vars->fd0 != 0)
		{
            if (vars->fd0 == -1)
                exit(1);
            dup2(vars->fd0, STDIN_FILENO);
            close(vars->fd0);
        }
		else if (prev_read_fd != 0)
		{
            dup2(prev_read_fd, STDIN_FILENO);
            close(prev_read_fd);
        }
        if (vars->fd1 != 1)
		{
            dup2(vars->fd1, STDOUT_FILENO);
            close(vars->fd1);
        }
		else if (commands[1])
		{
            dup2(vars->pipe_fd[1], STDOUT_FILENO);
            close(vars->pipe_fd[1]);
        }
        if (commands[1])
            close(vars->pipe_fd[0]);
        execute_command(vars, commands, envp);
    }
    if (prev_read_fd != 0)
        close(prev_read_fd);
    if (vars->fd1 != 1)
        close(vars->fd1);
    if (vars->fd0 != 0)
        close(vars->fd0);
    if (commands[1])
	{
        close(vars->pipe_fd[1]);
        vars->p0 = vars->pipe_fd[0];
    }
    cleanup_temp_file(vars);
}
