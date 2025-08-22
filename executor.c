/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:44:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/22 21:03:22 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **env_list_to_envp(t_env **env_list_ptr)
{
	t_env	*temp;
	char	**envp;
	int		count = 0;
	int		i = 0;

	if (!env_list_ptr || !*env_list_ptr)
		return (NULL);
    temp = *env_list_ptr;
	while (temp)
	{
		if (temp->key && temp->value)
			count++;
		temp = temp->next;
	}

	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);

	temp = *env_list_ptr;
	while (temp)
	{
		if (temp->key && temp->value)
		{
			int key_len = ft_strlen(temp->key);
			int value_len = ft_strlen(temp->value);
			int total_len = key_len + value_len + 2;

			envp[i] = malloc(total_len);
			if (!envp[i])
			{
                while (--i >= 0)
                    free(envp[i]);
                free(envp);
				// Hata durumunda daha önce allocate edilenleri freelemeyi unutma
				return (NULL);
			}
			ft_strcpy(envp[i], temp->key);
			ft_strcat(envp[i], "=");
			ft_strcat(envp[i], temp->value);
			i++;
		}
		temp = temp->next;
	}
	envp[i] = NULL;
	return (envp);
}

int execute_command(t_command *cmd)
{
    int saved_stdin;
    int saved_stdout;
    int exit_status;

    exit_status = 0;
    if (cmd == NULL)
    {
        set_exit_code(0);
        return(exit_status);
    }
    if (cmd->next)
        exit_status = execute_pipeline(cmd);

    else if (cmd->args && is_builtin(cmd->args[0]))
    {
        if (cmd->redirects)
        {
            saved_stdin = dup(STDIN_FILENO);
            saved_stdout = dup(STDOUT_FILENO);
            execute_redirects(cmd);
        }
        exit_status = execute_builtin(cmd);
        if (cmd->redirects)
        {
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
        }
        if (ft_strcmp(cmd->args[0], "exit") == 0)
        {
            free_commands(cmd);
            free_environment(init_env(NULL));
            exit(exit_status);

        }
    }
    else
    {
        exit_status = execute_simple_command(cmd);
    }
    cleanup_heredoc_pipes(cmd);
    set_exit_code(exit_status);
    return (exit_status);
}

int execute_simple_command(t_command *cmd)
{
    pid_t pid;
    int status;
	int a;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    else if (pid == 0)
    {
        restore_default_signals();
        if(cmd->redirects)
        {
            if (execute_redirects(cmd) == -1)
                exit(1);
        }
		if (cmd->args && cmd->args[0] != NULL)
        {
			a = 0;
            a = execve_command(cmd->args);
            if (a != 0)
            {
                    
                free_commands(cmd);
            	free_environment(init_env(NULL)); 
                exit(a);
            }
        }
		else
		{
			free_commands(cmd);
            free_environment(init_env(NULL)); 
		}
        exit(0);
    }
    else
    {
        configure_execution_signals();
        waitpid(pid, &status, 0);
        configure_prompt_signals();
        if (WIFEXITED(status))
        {
            return (WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            return(status);
        }
        else 
            return (1);
    }
}

static int  is_a_directory(char *path)
{
	struct stat	statbuf;

	if (stat(path, &statbuf) == 0)
		return (S_ISDIR(statbuf.st_mode));
	return (0);
}

int execve_command(char **args)
{
    char *cmd_path;
    char **envp;

    envp = env_list_to_envp(init_env(NULL));
    if (!args || !args[0])
	{
        return(127);
	}
	if (ft_strchr(args[0],'/'))
	{
		if (execve(args[0], args, envp) == -1)
		{
			ft_putstr_fd(args[0], 2);
			ft_putendl_fd(" : Is a directory", 2);
			free_environment(init_env(NULL));
			free_args(envp);
			if (is_a_directory(args[0]))
				return (127);
			else
				return (126);
			
			
		}
	}
	cmd_path = find_command_path(args[0]);
	if (!cmd_path)
    {
		if (ft_strchr(args[0], '/'))
    	{
        	if (is_a_directory(args[0]))
        	{
            	ft_putstr_fd(args[0], 2);
            	ft_putendl_fd(" : Is a directory", 2);
				free_environment(init_env(NULL));
				free_args(envp);
            	return(126);
        	}
        	else
        	{
            	ft_putstr_fd("minishell: ", 2);
            	ft_putstr_fd(args[0], 2);
            	ft_putstr_fd(": No such file or directory\n", 2);
				free_environment(init_env(NULL));
				free_args(envp);
            	return(127);
        	}
		}
		else
		{
			free_args(envp);
			free(cmd_path);
        	ft_putstr_fd("minishell: ", 2);
        	ft_putstr_fd(args[0], 2);
       		ft_putstr_fd(": command not found\n", 2);
        	return(127);
		}
    }
	if (execve(cmd_path, args, envp) == -1)
	{
		free_args(envp);
		free(cmd_path);
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        return(127);
    }
    return (0);
}


char *find_command_path(char *cmd)
{
    char *path_env;
    char **paths;
    char *full_path;
    int i;
    
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

int execute_pipeline(t_command *cmd)
{
    int cmd_count;
    pid_t *pids;
    int **pipes;
    int exit_status;
    int i;
    int a;
    t_command *head;

    cmd_count = count_commands(cmd);
    head = cmd;
    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
        return (1);
    
    pipes = create_pipes(cmd_count - 1);
    if (!pipes && cmd_count > 1)
    {
        free(pids);
        return (1);
    }
    i = 0;
    while (cmd && i < cmd_count)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork");
            cleanup_pipeline(pids, pipes, cmd_count);
            return (1);
        }
        
        if (pids[i] == 0)
        {
            restore_default_signals();
            setup_pipe_redirections(pipes, i, cmd_count);
            close_all_pipes(pipes, cmd_count - 1);

            if (execute_redirects(cmd) == -1)
                exit(1);
            if (cmd->args[0] && is_builtin(cmd->args[0]))
            {
                exit_status = execute_builtin(cmd);
                free_commands(head);
                cleanup_pipeline(pids, pipes, cmd_count);
                free_environment(init_env(NULL));
                exit(exit_status);
            }
            if (cmd->args[0] != NULL)
            {
                a = execve_command(cmd->args);
                if (a != 0)
                  {
                    
                    free_commands(head);
                    free_environment(init_env(NULL));  
                    
                    cleanup_pipeline(pids, pipes, cmd_count);
                    exit(a);
                  }
            }
            exit(0);
        }
        cmd = cmd->next;
        i++;
    }
    close_all_pipes(pipes, cmd_count - 1);
    configure_execution_signals();
    exit_status = wait_for_children(pids, cmd_count);
    configure_prompt_signals();
    cleanup_pipeline(pids, pipes, cmd_count);
    set_exit_code(exit_status);
    return (exit_status);
}

int **create_pipes(int pipe_count)
{
    int **pipes;
    int i;
    
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
            while (--i >= 0)
            {
                close(pipes[i][0]);
                close(pipes[i][1]);
                free(pipes[i]);
            }
            free(pipes);
            return (NULL);
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
