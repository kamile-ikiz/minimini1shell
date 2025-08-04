/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:34:40 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/04 20:06:20 by kikiz            ###   ########.fr       */
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

// ANSI Color Codes for token display
#define RESET_COLOR     "\033[0m"
#define RED             "\033[1;31m"
#define GREEN           "\033[1;32m"
#define YELLOW          "\033[1;33m"
#define BLUE            "\033[1;34m"
#define MAGENTA         "\033[1;35m"
#define CYAN            "\033[1;36m"
#define WHITE           "\033[1;37m"
#define GRAY            "\033[0;37m"
#define BOLD            "\033[1m"
#define PROMPT          "\001\033[38;5;205m\002minimini1shell$ \001\033[0m\002"
#define BUFFER_SIZE     1024

typedef enum {
    TOKEN_WORD,           // Regular words like "ls", "file.txt"
    TOKEN_PIPE,           // |
    TOKEN_REDIRECT_IN,    // <
    TOKEN_REDIRECT_OUT,   // >
    TOKEN_REDIRECT_APPEND,// >>
    TOKEN_HEREDOC,        // <<
    TOKEN_ERROR           // Parsing error
} token_type_t;

// typedef struct minishell
// {
//     command_t   *commands;
//     char        **envp;
//     int         exit_code;
//     int         should_exit;
//     pid_t       *pids;
//     int         *pipes;
//     int         pipe_count;
// }   minishell_t;

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
typedef struct command {
    char **args;              // Array of command arguments ["ls", "-la", NULL]
    int argc;                 // Number of arguments
    redirect_t *redirects;    //redirect list
    struct command *next;     // Next command in pipeline
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

//func prototypes
//-----------------free_functions---------------

void free_tokens(token_t *tokens);
void free_command(command_t *cmd);
void free_pipeline(pipeline_t *pipeline);
void free_segments(segment_t *segments);
void	free_redirects(redirect_t *redirects);
//---------------------------------------------------------
command_t	*new_command(void);
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
int     setup_heredoc_redirect(command_t *cmd, char *delimiter);
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
int	parse_command_or_redirect(segment_t *segment, command_t *cmd);

#endif 