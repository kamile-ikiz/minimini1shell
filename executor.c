/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:44:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/08 14:47:39 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_strcat(char *dest, const char *src)
{
	int i = 0;
	int j = 0;
	while (dest[i])
		i++;
	while (src[j])
		dest[i++] = src[j++];
	dest[i] = '\0';
	return (dest);
}
char *ft_strcpy(char *dst, const char *src)
{
	int i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char **env_list_to_envp(t_env **env_list_ptr)
{
	t_env	*temp;
	char	**envp;
	int		count = 0;
	int		i = 0;

	if (!env_list_ptr || !*env_list_ptr)
		return (NULL);

	temp = *env_list_ptr;

	// Geçerli (key && value) çiftlerini say
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
			int total_len = key_len + value_len + 2; // '=' + '\0'

			envp[i] = malloc(total_len);
			if (!envp[i])
			{
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


// Ana executor fonksiyonu
int execute_command(command_t *cmd)
{
    if (!cmd)
        return (1);
    if (is_builtin(cmd->args[0]))
        return (execute_builtin(cmd));
    if (cmd->next)
        return (execute_pipeline(cmd));
    return (execute_simple_command(cmd));
}

// Basit komut çalıştırma (pipe olmadan)
int execute_simple_command(command_t *cmd)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (1);
    }
    
    if (pid == 0) // Child process
    {
        // Redirectionları ayarla
        if (execute_redirects(cmd) == -1)
            exit(1);
        
        // Komutu çalıştır
        if (execve_command(cmd->args) == -1)
            exit(127); // Command not found
    }
    else // Parent process
    {
        // Child'ın bitmesini bekle
        waitpid(pid, &status, 0);
        return (WEXITSTATUS(status));
    }
    
    return (0);
}

// Execve ile komutu çalıştırma
int execve_command(char **args)
{
    char *cmd_path;
    char **envp;

    envp = env_list_to_envp(init_env(NULL));
    if (!args || !args[0])
        return (-1);
    
    // Eğer absolute/relative path ise direkt çalıştır
    if (ft_strchr(args[0], '/'))
    {
        if (access(args[0], X_OK) == 0)
            execve(args[0], args, envp);
        else
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(args[0], 2);
            ft_putstr_fd(": No such file or directory\n", 2);
            return (-1);
        }
    }
    
    // PATH'de ara
    cmd_path = find_command_path(args[0]);
    if (!cmd_path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        return (-1);
    }
    
    execve(cmd_path, args, envp);
    free(cmd_path);
    
    // Execve başarısız olursa
    perror("execve");
    return (-1);
}

// PATH'de komut arama
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

// Pipeline çalıştırma (birden fazla pipe)
int execute_pipeline(command_t *cmd)
{
    int cmd_count;
    pid_t *pids;
    int **pipes;
    int status;
    int i;
    
    // Komut sayısını say
    cmd_count = count_commands(cmd);
    
    // PID array'i oluştur
    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
        return (1);
    
    // Pipe array'i oluştur
    pipes = create_pipes(cmd_count - 1);
    if (!pipes && cmd_count > 1)
    {
        free(pids);
        return (1);
    }
    
    // Her komut için process oluştur
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
        
        if (pids[i] == 0) // Child process
        {
            setup_pipe_redirections(pipes, i, cmd_count);
            close_all_pipes(pipes, cmd_count - 1);
            
            // Redirectionları ayarla
            if (execute_redirects(cmd) == -1)
                exit(1);
            
            // Builtin mı kontrol et
            if (is_builtin(cmd->args[0]))
                exit(execute_builtin(cmd));
            
            // Normal komut çalıştır
            if (execve_command(cmd->args) == -1)
                exit(127);
        }
        
        cmd = cmd->next;
        i++;
    }
    
    // Parent process: tüm pipe'ları kapat
    close_all_pipes(pipes, cmd_count - 1);
    
    // Tüm child'ların bitmesini bekle
    status = wait_for_children(pids, cmd_count);
    
    // Cleanup
    cleanup_pipeline(pids, pipes, cmd_count);
    
    return (status);
}

// Pipe array'i oluşturma
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
            // Önceki pipe'ları temizle
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

// Pipe redirectionlarını ayarlama
void setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count)
{
      if (cmd_index > 0)
        dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
    if (cmd_index < cmd_count - 1)
        dup2(pipes[cmd_index][1], STDOUT_FILENO);
}

// Redirectionları ayarlama (< > << >>)
// int setup_redirections(command_t *cmd)
// {
//     t_redirect *redirect;
    
//     redirect = cmd->redirects;
//     while (redirect)
//     {
//         if (redirect->type == REDIRECT_INPUT) // <
//         {
//             int fd = open(redirect->file, O_RDONLY);
//             if (fd == -1)
//             {
//                 perror(redirect->file);
//                 return (-1);
//             }
//             dup2(fd, STDIN_FILENO);
//             close(fd);
//         }
//         else if (redirect->type == REDIRECT_OUTPUT) // >
//         {
//             int fd = open(redirect->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (fd == -1)
//             {
//                 perror(redirect->file);
//                 return (-1);
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         else if (redirect->type == REDIRECT_APPEND) // >>
//         {
//             int fd = open(redirect->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
//             if (fd == -1)
//             {
//                 perror(redirect->file);
//                 return (-1);
//             }
//             dup2(fd, STDOUT_FILENO);
//             close(fd);
//         }
//         else if (redirect->type == REDIRECT_HEREDOC) // <<
//         {
//             if (setup_heredoc(redirect->delimiter) == -1)
//                 return (-1);
//         }
        
//         redirect = redirect->next;
//     }
    
//     return (0);
// }

// Heredoc ayarlama
// int setup_heredoc(char *delimiter)
// {
//     int pipefd[2];
//     char *line;
    
//     if (pipe(pipefd) == -1)
//     {
//         perror("pipe");
//         return (-1);
//     }
    
//     // Heredoc input'u oku ve pipe'a yaz
//     while (1)
//     {
//         line = readline("> ");
//         if (!line || ft_strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             break;
//         }
        
//         write(pipefd[1], line, ft_strlen(line));
//         write(pipefd[1], "\n", 1);
//         free(line);
//     }
    
//     close(pipefd[1]);
//     dup2(pipefd[0], STDIN_FILENO);
//     close(pipefd[0]);
    
//     return (0);
// }

// Tüm pipe'ları kapatma
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

// Child processların bitmesini bekleme
int wait_for_children(pid_t *pids, int cmd_count)
{
    int status;
    int exit_status = 0;
    int i;
    
    i = 0;
    while (i < cmd_count)
    {
        waitpid(pids[i], &status, 0);
        // Son komutun exit status'ını döndür
        if (i == cmd_count - 1)
            exit_status = WEXITSTATUS(status);
        i++;
    }
    
    return (exit_status);
}

// Cleanup fonksiyonu
void cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count)
{
    int i;
    
    // Pipe'ları temizle
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
    
    // PID array'ini temizle
    free(pids);
}

// Utility fonksiyonlar
int count_commands(command_t *cmd)
{
    int count = 0;
    
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    
    return (count);
}
