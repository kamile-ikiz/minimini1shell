/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:44:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/07 13:18:52 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Ana executor fonksiyonu
int execute_command(command_t *cmd, char **envp)
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
        if (execute_redirects(cmd->redirects) == -1)
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
    
    if (!args || !args[0])
        return (-1);
    
    // Eğer absolute/relative path ise direkt çalıştır
    if (ft_strchr(args[0], '/'))
    {
        if (access(args[0], X_OK) == 0)
            execve(args[0], args, init_env(NULL));
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
    
    execve(cmd_path, args, init_env(NULL));
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
    
    // PATH environment variable'ını al
    path_env = get_env_value("PATH", init_env(NULL));
    if (!path_env)
        return (NULL);
    
    // PATH'i ':' ile böl
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    
    i = 0;
    while (paths[i])
    {
        // Her path ile komut adını birleştir
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
            cleanup_pipeline(pids, pipes, cmd_count, i);
            return (1);
        }
        
        if (pids[i] == 0) // Child process
        {
            setup_pipe_redirections(cmd, pipes, i, cmd_count);
            close_all_pipes(pipes, cmd_count - 1);
            
            // Redirectionları ayarla
            if (execute_redirects(cmd->redirects) == -1)
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
    cleanup_pipeline(pids, pipes, cmd_count, cmd_count);
    
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
void setup_pipe_redirections(command_t *cmd, int **pipes, int cmd_index, int cmd_count)
{
    // İlk komut değilse, önceki pipe'dan oku
    if (cmd_index > 0)
    {
        dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
    }
    
    // Son komut değilse, sonraki pipe'a yaz
    if (cmd_index < cmd_count - 1)
    {
        dup2(pipes[cmd_index][1], STDOUT_FILENO);
    }
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
void cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count, int created_processes)
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

char *ft_strjoin_three(char *s1, char *s2, char *s3)
{
    char *temp;
    char *result;
    
    temp = ft_strjoin(s1, s2);
    if (!temp)
        return (NULL);
    
    result = ft_strjoin(temp, s3);
    free(temp);
    
    return (result);
}