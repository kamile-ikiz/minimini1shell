/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:34:40 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/20 18:29:21 by kikiz            ###   ########.fr       */
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
# include <sys/stat.h>

#define PROMPT          "\001\033[38;5;205m\002minimini1shell$ \001\033[0m\002"
#define BUFFER_SIZE     1024

extern volatile sig_atomic_t g_signal_flag;

typedef enum {
    TOKEN_WORD,           // Regular words like "ls", "file.txt"
    TOKEN_PIPE,           // |
    TOKEN_REDIRECT_IN,    // <
    TOKEN_REDIRECT_OUT,   // >
    TOKEN_REDIRECT_APPEND,// >>
    TOKEN_HEREDOC,        // <<
} t_token_type;

//token struct
typedef struct s_token {
    t_token_type type;    // What kind of token this is
    char *value;          // The actual text content
    struct s_token *next;   // Pointer to next token (linked list)
    int expand_mode;
    int was_quoted;
} t_token;

//segment struct
typedef struct s_segment
{
	t_token	            *tokens;
	int                 token_count;
	struct s_segment      *next;
}	t_segment;


//redirect struct
typedef struct s_redirect
{
	t_token_type		type;
	char				*filename;
    int                 heredoc_pipe_fd;
	struct s_redirect	*next;
}	t_redirect;

//command struct

// typedef enum e_cmd_type
// {
//     CMD_NOT_FOUND,
//     CMD_BUILTIN,
//     CMD_EXTERNAL,
//     PERMISSON_DENIED,
//     NO_PATH,
// }   t_cmd_type;


typedef struct s_command {
    char **args;              // Array of command arguments ["ls", "-la", NULL]
    int argc;                 // Number of arguments
    t_redirect *redirects;    //redirect list
    struct s_command *next;     // Next command in pipeline
    // t_cmd_type  type;
} t_command;

//pipeline struct
typedef struct s_pipeline {
    t_command *commands;   // Linked list of commands (cmd1 | cmd2 | cmd3)
    int background;        // Whether to run in background (&)
    struct s_pipeline *next; // Next pipeline in case of multiple pipelines
    char opertr;           // Operator connecting pipelines
} t_pipeline;

//parser state
typedef struct s_parser {
    t_token *token_list;
    char *inp;
    int pos;
    int error;
    char *error_msg;
} t_parser;

//data expand edilecekse kullanılıyor
typedef struct s_expand_data
{
	char	*before;
	char	*after;
	char	*result;
	int		new_pos;
}	t_expand_data;

// typedef struct s_minishell
// {
//     t_segment   *segment;
//     char        **envp;
//     int         exit_code;
//     int         should_exit;
//     pid_t       *pids;
//     int         *pipes;
//     int         pipe_count;
// }   t_minishell;


//func prototypes
//-----------------free_functions---------------
char *remove_quotes(const char *input);
// void print_parser_error(t_parser *parser);
// void free_parser(t_parser *parser);
void free_tokens(t_token *tokens);
void free_command(t_command *cmd);
void free_pipeline(t_command *pipeline);
void free_segments(t_segment *segments);
void	free_redirects(t_redirect *redirects);
void	free_array(char **array);

//------------------TOKEN------------------------------
t_token *new_token(t_token_type type, char *value);
t_token    *token_lst(t_token **head, t_token *token);
void	init_parser(t_parser *parser, char *input);
t_token *tokenize(char *input);
//-----------------SEGMENT----------------------------
t_segment	*create_single_segment(t_token *start_token, int count);
int count_tokens_until_pipe(t_token *token_list);

void skip_whitespace(t_parser *parser);
void print_tokens_fancy(t_token *tokens);
void print_tokens_simple(t_token *tokens);
char *parse_quotes(t_parser *parser, char quote);
char *parse_word(t_parser *parser);
int is_space(char c);
int check_all_syntax(t_token *head);
char *expand_or_not(t_parser *parser, char status);
char    *ft_strjoin_free(char *s1, char *s2);
char    *get_env_value(char *var_name, t_env **env_list_ptr);
int	get_token_count(t_token *head);
//--------------------heredoc------------------------------------
char	*expand(const char *input);
char	*append_variable(char *line, int *i, char *result);
char	*append_normal_char(char *line, int i, char *result);
int     setup_heredoc_redirect(char *delimiter);
char	*handle_heredoc_delimiter(char *delimiter);
char	*get_next_line(int fd);
int prepare_heredocs(t_command *cmd_list);
void cleanup_heredoc_pipes(t_command *cmd_list);
int	read_heredoc(const char *delim, int fd, int expand);
//-------------------redirect list or word list-------------------------------------
int     handle_redirect_pair(t_token *redirect_token, t_token *filename,
	t_command *cmd);
int handle_command_pair(t_token *word, t_command *cmd);
int	is_redirect_token(t_token token);
t_command	*create_command(void);

t_segment	*split_tokens_by_pipe(t_token *token_list);
int	parse_command_or_redirect(t_segment *segment, t_command **cmd_ptr);
int	execute_redirects(t_command *cmd);
int	handle_single_redirect(t_redirect *redirect);
int	handle_append_redirect(char *filename);
int	handle_output_redirect(char *filename);
int	handle_input_redirect(char *filename);
int	handle_heredoc_redirect(char *delimiter);
int	heredoc_parent_process(int pipe_fd[2], pid_t pid);
int	heredoc_child_process(int pipe_fd[2], char *delimiter);

void	print_pipeline(t_command *pipeline_head);
t_command	*parse_input(char *input);
int execute_command(t_command *cmd);
//--------------------SIGNALS------------------------------------------------
extern volatile sig_atomic_t g_signal_flag;
void    assign_signal_handler(int signal_type, void (*callback)(int));
void	interrupt_callback_prompt(int signal_num);
int    configure_prompt_signals(void);
void	interrupt_callback_execution(int signal_num);
void	quit_callback_execution(int signal_num);
void	configure_execution_signals(void);
void	interrupt_callback_heredoc(int signal_num);
void    configure_heredoc_signals(void);
// void	validate_signal_state(t_minishell *shell_ctx);
// void	process_interrupt_during_input(t_minishell *shell_ctx, char **user_input);
// void	process_interrupt_after_input(t_minishell *shell_ctx, char *user_input);
// void	process_interrupt_during_parse(t_minishell *shell_ctx);
void	restore_default_signals(void);

int execute_simple_command(t_command *cmd);
int execve_command(char **args);
char *find_command_path(char *cmd);
int execute_pipeline(t_command *cmd);
int **create_pipes(int pipe_count);
void setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count);
void close_all_pipes(int **pipes, int pipe_count);
int wait_for_children(pid_t *pids, int cmd_count);
void cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count);
int count_commands(t_command *cmd);
char **env_list_to_envp(t_env **env_list);
char	*parse_unquoted_segment(t_parser *parser);
int	is_word_delimiter(char c);
void	*set_parser_error(t_parser *parser, char *msg, void *to_free);
int	parse_error(t_command **cmd_ptr);
char	*expand_all_variables(char *arg, t_env **env_list_ptr);
void	add_segment(t_segment **head, t_segment *new_segment);
int	append_segment(char **word_ptr, char *segment);
char	*expand_variable_parts(char *arg, char *env_value, int dollar_pos, int var_end);
void    print_syntax_error(char *token);
int	check_pipe_syntax(t_token *tokens, int token_count);
t_token	*token_get_last(t_token *head);
int	read_heredoc_until_delimiter(const char *delimiter,
		int write_fd, int expand_vars);

void set_exit_code(int status);
int get_exit_code(void);

#endif 