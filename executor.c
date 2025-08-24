/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:44:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/24 15:27:53 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_valid_envs(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		if (env_list->key && env_list->value)
			count++;
		env_list = env_list->next;
	}
	return (count);
}

static char	*create_env_string(t_env *node)
{
	char	*env_str;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(node->key);
	value_len = ft_strlen(node->value);
	env_str = malloc(key_len + value_len + 2);
	if (!env_str)
		return (NULL);
	ft_memcpy(env_str, node->key, key_len);
	env_str[key_len] = '=';
	ft_memcpy(env_str + key_len + 1, node->value, value_len);
	env_str[key_len + value_len + 1] = '\0';
	return (env_str);
}

static int	populate_envp_array(char **envp, t_env *env_list)
{
	int	i;

	i = 0;
	while (env_list)
	{
		if (env_list->key && env_list->value)
		{
			envp[i] = create_env_string(env_list);
			if (!envp[i])
			{
				while (--i >= 0)
					free(envp[i]);
				return (-1);
			}
			i++;
		}
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (0);
}

char	**env_list_to_envp(t_env **env_list_ptr)
{
	char	**envp;
	int		count;

	if (!env_list_ptr || !*env_list_ptr)
		return (NULL);
	count = count_valid_envs(*env_list_ptr);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (populate_envp_array(envp, *env_list_ptr) == -1)
	{
		free(envp);
		return (NULL);
	}
	return (envp);
}

static int	backup_and_apply_redirects(t_command *cmd, int *stdin_backup,
	int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
	{
		perror("dup");
		return (-1);
	}
	if (execute_redirects(cmd) == -1)
		return (-1);
	return (0);
}

static void	restore_fds(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	execute_builtin_command(t_command *cmd)
{
	int	exit_status;
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = -1;
	if (cmd->redirects)
	{
		if (backup_and_apply_redirects(cmd, &saved_stdin, &saved_stdout) == -1)
			return (1);
	}
	exit_status = execute_builtin(cmd);
	if (saved_stdin != -1)
		restore_fds(saved_stdin, saved_stdout);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		free_commands(cmd);
		free_environment(init_env(NULL));
		exit(exit_status);
	}
	return (exit_status);
}

int	execute_command(t_command *cmd)
{
	int	exit_status;

	exit_status = 0;
	if (cmd == NULL)
		return (set_exit_code(0), 0);
	if (cmd->next)
		exit_status = execute_pipeline(cmd);
	else if (cmd->args && is_builtin(cmd->args[0]))
		exit_status = execute_builtin_command(cmd);
	else
		exit_status = execute_simple_command(cmd);
	cleanup_heredoc_pipes(cmd);
	set_exit_code(exit_status);
	return (exit_status);
}

static void	child_process_routine(t_command *cmd)
{
	int	exec_status;

	restore_default_signals();
	if (cmd->redirects)
	{
		if (execute_redirects(cmd) == -1)
			exit(1);
	}
	if (cmd->args && cmd->args[0])
	{
		exec_status = execve_command(cmd->args);
		free_commands(cmd);
		free_environment(init_env(NULL));
		exit(exec_status);
	}
	free_commands(cmd);
	free_environment(init_env(NULL));
	exit(0);
}

static int	parent_process_routine(pid_t child_pid)
{
	int	status;

	configure_execution_signals();
	waitpid(child_pid, &status, 0);
	configure_prompt_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (status);
	return (1);
}

int	execute_simple_command(t_command *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		child_process_routine(cmd);
	}
	return (parent_process_routine(pid));
}

static int	is_a_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0)
		return (S_ISDIR(statbuf.st_mode));
	return (0);
}

static int	handle_exec_error(char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	if (is_a_directory(command))
	{
		ft_putendl_fd(": Is a directory", 2);
		return (126);
	}
	if (ft_strchr(command, '/'))
	{
		ft_putendl_fd(": No such file or directory", 2);
		return (127);
	}
	ft_putendl_fd(": command not found", 2);
	return (127);
}

static int	execute_direct_path(char **args, char **envp)
{
	int	exit_code;

	execve(args[0], args, envp);
	exit_code = handle_exec_error(args[0]);
	free_args(envp);
	free_environment(init_env(NULL));
	return (exit_code);
}

static int	execute_path_search(char **args, char **envp)
{
	char	*cmd_path;
	int		exit_code;

	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
	{
		exit_code = handle_exec_error(args[0]);
		free_args(envp);
		free_environment(init_env(NULL));
		return (exit_code);
	}
	execve(cmd_path, args, envp);
	exit_code = handle_exec_error(args[0]);
	free(cmd_path);
	free_args(envp);
	free_environment(init_env(NULL));
	return (exit_code);
}

int	execve_command(char **args)
{
	char	**envp;

	if (!args || !args[0])
		return (127);
	envp = env_list_to_envp(init_env(NULL));
	if (!envp && *init_env(NULL) != NULL)
		return (1);
	if (ft_strchr(args[0], '/'))
		return (execute_direct_path(args, envp));
	else
		return (execute_path_search(args, envp));
}

char	*find_command_path(char *cmd)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;
    
	path_env = get_env_value("PATH", init_env(NULL));
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
    }
    free_array(paths);
    return (NULL);
}

// void	free_pipeline_data(t_pipeline_data *data)
// {
// 	int	i;

// 	if (!data)
// 		return;
// 	if (data->pids)
// 	{
// 		free(data->pids);
// 		data->pids = NULL;
// 	}
// 	if (data->pipes)
// 	{
// 		i = 0;
// 		while (i < data->cmd_count - 1)
// 		{
// 			if (data->pipes[i])
// 				free(data->pipes[i]);
// 			i++;
// 		}
// 		free(data->pipes);
// 		data->pipes = NULL;
// 	}
// }

static int	init_pipeline_data(t_command *cmd, t_pipeline_data *data)
{
	data->head = cmd;
	data->cmd_count = count_commands(cmd);
	data->pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!data->pids)
		return (-1);
	if (data->cmd_count > 1)
	{
		data->pipes = create_pipes(data->cmd_count - 1);
		if (!data->pipes)
		{
			free(data->pids);
			return (-1);
		}
	}
	else
		data->pipes = NULL;
	return (0);
}

static void	pipeline_child_routine(t_command *cmd, t_pipeline_data *data, int i)
{
	int	exit_status;	
	restore_default_signals();
	setup_pipe_redirections(data->pipes, i, data->cmd_count);
	close_all_pipes(data->pipes, data->cmd_count - 1);
	if (execute_redirects(cmd) == -1)
		exit(1);
	if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
	{
		exit_status = execute_builtin(cmd);
		free_commands(data->head);
		cleanup_pipeline(data->pids, data->pipes, data->cmd_count);
		free_environment(init_env(NULL));
		exit(exit_status);
	}
	if (cmd->args && cmd->args[0])
	{
		exit_status = execve_command(cmd->args);
		if (exit_status == 127 || exit_status == 126)
		{
			free_commands(data->head);
			cleanup_pipeline(data->pids, data->pipes, data->cmd_count);
		}
		exit(exit_status);
	}
	exit(0);
}

static int	launch_processes(t_command *cmd, t_pipeline_data *data)
{
	int	i;

	i = 0;
	while (cmd && i < data->cmd_count)
	{
		data->pids[i] = fork();
		if (data->pids[i] == -1)
		{
			perror("fork");
			return (-1);
		}
		if (data->pids[i] == 0)
			pipeline_child_routine(cmd, data, i);
		cmd = cmd->next;
		i++;
	}
	return (0);
}

int	execute_pipeline(t_command *cmd)
{
	t_pipeline_data	data;
	int				exit_status;

	if (init_pipeline_data(cmd, &data) == -1)
		return (1);
	if (launch_processes(cmd, &data) == -1)
	{
		cleanup_pipeline(data.pids, data.pipes, data.cmd_count);
		return (1);
	}
	close_all_pipes(data.pipes, data.cmd_count - 1);
	configure_execution_signals();
	exit_status = wait_for_children(data.pids, data.cmd_count);
	configure_prompt_signals();
	cleanup_pipeline(data.pids, data.pipes, data.cmd_count);
	set_exit_code(exit_status);
	return (exit_status);
}

static int	**cleanup_pipes_on_error(int **pipes, int count)
{
	while (--count >= 0)
	{
		close(pipes[count][0]);
		close(pipes[count][1]);
		free(pipes[count]);
	}
	free(pipes);
	return (NULL);
}

int	**create_pipes(int pipe_count)
{
	int	**pipes;
	int	i;

	if (pipe_count <= 0)
		return (NULL);
	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (cleanup_pipes_on_error(pipes, i));
		}
		i++;
	}
	return (pipes);
}

void setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count)
{
      if (cmd_index > 0)
        dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
    if (cmd_index < cmd_count - 1)
        dup2(pipes[cmd_index][1], STDOUT_FILENO);
}

void close_all_pipes(int **pipes, int pipe_count)
{
    int i;
    
    i = 0;
    while (i < pipe_count)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

int wait_for_children(pid_t *pids, int cmd_count)
{
    int status;
    int exit_status = 0;
    int i;
    
    i = 0;
    while (i < cmd_count)
    {
        waitpid(pids[i], &status, 0);
        if (i == cmd_count - 1)
            exit_status = WEXITSTATUS(status);
        i++;
    }
    
    return (exit_status);
}

void cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count)
{
    int i;

    if (pipes)
    {
        i = 0;
        while (i < cmd_count - 1)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
            free(pipes[i]);
            i++;
        }
        free(pipes);
    }
    free(pids);
}

int count_commands(t_command *cmd)
{
    int count = 0;
    
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    
    return (count);
}
