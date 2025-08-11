/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:34:40 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/11 21:10:51 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft/libft.h"
#include "builtins/builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <termios.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT          "\001\033[38;5;205m\002minimini1shell$ \001\033[0m\002"
#define BUFFER_SIZE     1024

typedef enum {
    TOKEN_WORD,           // Regular words like "ls", "file.txt"
    TOKEN_PIPE,           // |
    TOKEN_REDIRECT_IN,    // <
    TOKEN_REDIRECT_OUT,   // >
    TOKEN_REDIRECT_APPEND,// >>
    TOKEN_HEREDOC,        // <<
} token_type_t;

//token struct
typedef struct token {
    token_type_t type;    // What kind of token this is
    char *value;          // The actual text content
    struct token *next;   // Pointer to next token (linked list)
} token_t;

//segment struct
typedef struct segment
{
	token_t	            *tokens;
	int                 token_count;
	struct segment      *next;
}	segment_t;


//redirect struct
typedef struct redirect
{
	token_type_t		type;
	char				*filename;
	struct redirect	*next;
}	redirect_t;

//command struct

typedef enum e_cmd_type
{
    CMD_NOT_FOUND,
    CMD_BUILTIN,
    CMD_EXTERNAL,
    PERMISSON_DENIED,
    NO_PATH,
}   t_cmd_type;


typedef struct command {
    char **args;              // Array of command arguments ["ls", "-la", NULL]
    int argc;                 // Number of arguments
    redirect_t *redirects;    //redirect list
    struct command *next;     // Next command in pipeline
    t_cmd_type  type;
} command_t;

//pipeline struct
typedef struct pipeline {
    command_t *commands;   // Linked list of commands (cmd1 | cmd2 | cmd3)
    int background;        // Whether to run in background (&)
    struct pipeline *next; // Next pipeline in case of multiple pipelines
    char opertr;           // Operator connecting pipelines
} pipeline_t;

//parser state
typedef struct parser {
    token_t *tokens;
    token_t *current;
    char *inp;
    int pos;
    int error;
    char *error_msg;
} parser_t;

//data expand edilecekse kullanılıyor
typedef struct s_expand_data
{
	char	*before;
	char	*after;
	char	*result;
	int		new_pos;
}	t_expand_data;

typedef struct minishell
{
    segment_t   *segment;
    char        **envp;
    int         exit_code;
    int         should_exit;
    pid_t       *pids;
    int         *pipes;
    int         pipe_count;
}   minishell_t;


//func prototypes
//-----------------free_functions---------------
char *remove_quotes(const char *input);
void print_parser_error(parser_t *parser);
void free_parser(parser_t *parser);
void free_tokens(token_t *tokens);
void free_command(command_t *cmd);
void free_pipeline(command_t *pipeline);
void free_segments(segment_t *segments);
void	free_redirects(redirect_t *redirects);
void	free_array(char **array);
//---------------------------------------------------------
command_t	*create_command(void);
token_t *new_token(token_type_t type, char *value);
void    token_lst(token_t **head, token_t *token);
void skip_whitespace(parser_t *parser);
void print_tokens_fancy(token_t *tokens);
void print_tokens_simple(token_t *tokens);
char *parse_quotes(parser_t *parser, char quote);
char *parse_word(parser_t *parser);
token_t *tokenize(char *input);
int is_space(char c);
int check_all_syntax(token_t *head);
char *expand_or_not(parser_t *parser, char status);
char    *ft_strjoin_free(char *s1, char *s2);
char    *get_env_value(char *var_name, t_env **env_list_ptr);
int	get_token_count(token_t *head);
//--------------------heredoc------------------------------------
char	*append_variable(char *line, int *i, char *result);
char	*append_normal_char(char *line, int i, char *result);
int     setup_heredoc_redirect(char *delimiter);
char	*handle_heredoc_delimiter(char *delimiter);
char	*get_next_line(int fd);
//-------------------redirect list or word list-------------------------------------
redirect_t	*create_redirect(token_type_t type, char *filename);
void	add_redirect(redirect_t **head, redirect_t *new_redirect);
int     handle_redirect_pair(token_t *redirect_token, token_t *filename,
	command_t *cmd);
int handle_command_pair(token_t *word, command_t *cmd);
int	is_redirect_token(token_t token);
segment_t	*split_tokens_by_pipe(token_t *token_list);
int	parse_command_or_redirect(segment_t *segment, command_t **cmd_ptr);
int	execute_redirects(command_t *cmd);
int	handle_single_redirect(redirect_t *redirect);
int	handle_append_redirect(char *filename);
int	handle_output_redirect(char *filename);
int	handle_input_redirect(char *filename);
int	handle_heredoc_redirect(char *delimiter);
int	heredoc_parent_process(int pipe_fd[2], pid_t pid);
int	heredoc_child_process(int pipe_fd[2], char *delimiter);

void	print_pipeline(command_t *pipeline_head);
command_t	*parse_input(char *input);
int execute_command(command_t *cmd);
//--------------------SIGNALS------------------------------------------------
extern volatile sig_atomic_t g_signal_flag;
void    assign_signal_handler(int signal_type, void (*callback)(int));
void	interrupt_callback_prompt(int signal_num);
void    configure_prompt_signals(void);
void	interrupt_callback_execution(int signal_num);
void	quit_callback_execution(int signal_num);
void	configure_execution_signals(void);
void	interrupt_callback_heredoc(int signal_num);
void	configure_heredoc_signals(void);
void	validate_signal_state(minishell_t *shell_ctx);
void	process_interrupt_during_input(minishell_t *shell_ctx, char **user_input);
void	process_interrupt_after_input(minishell_t *shell_ctx, char *user_input);
void	process_interrupt_during_parse(minishell_t *shell_ctx);
void	restore_default_signals(void);

int execute_simple_command(command_t *cmd);
int execve_command(char **args);
char *find_command_path(char *cmd);
int execute_pipeline(command_t *cmd);
int **create_pipes(int pipe_count);
void setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count);
void close_all_pipes(int **pipes, int pipe_count);
int wait_for_children(pid_t *pids, int cmd_count);
void cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count);
int count_commands(command_t *cmd);
char **env_list_to_envp(t_env **env_list);
char	*parse_unquoted_segment(parser_t *parser);
char	*ft_join_and_free(char *s1, char *s2);
int	is_word_delimiter(char c);
void	*set_parser_error(parser_t *parser, char *msg, void *to_free);
int	parse_error(command_t **cmd_ptr);
#endif 