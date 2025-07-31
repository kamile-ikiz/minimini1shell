/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:34:40 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/31 15:43:21 by kikiz            ###   ########.fr       */
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
    TOKEN_EOF,            // End of input
    TOKEN_ERROR           // Parsing error
} token_type_t;

//command struct
typedef struct command {
    char **args;              // Array of command arguments ["ls", "-la", NULL]
    int argc;                 // Number of arguments
    char *input_file;         // File for input redirection (< file)
    char *output_file;        // File for output redirection (> file)
    int append_mode;          // 1 if >>, 0 if >
    char *heredoc_delimiter;  // Delimiter for heredoc (<<)
    struct command *next;     // Next command in pipeline
} command_t;

//pipeline struct
typedef struct pipeline {
    command_t *commands;   // Linked list of commands (cmd1 | cmd2 | cmd3)
    int background;        // Whether to run in background (&)
    struct pipeline *next; // Next pipeline in case of multiple pipelines
    char opertr;           // Operator connecting pipelines
} pipeline_t;

//token struct
typedef struct token {
    token_type_t type;    // What kind of token this is
    char *value;          // The actual text content
    struct token *next;   // Pointer to next token (linked list)
} token_t;

//parser state
typedef struct parser {
    token_t *tokens;
    token_t *current;
    char *inp;
    int pos;
    int error;
    char *error_msg;
} parser_t;

//func prototypes
token_t *new_token(token_type_t type, char *value);
void    token_lst(token_t **head, token_t *token);
void skip_whitespace(parser_t *parser);
void free_tokens(token_t *tokens);
void free_command(command_t *cmd);
void free_pipeline(pipeline_t *pipeline);
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
//--------------------heredoc------------------------------------
char	*append_variable(char *line, int *i, char *result);
char	*append_normal_char(char *line, int i, char *result);
int     setup_heredoc_redirect(command_t *cmd, char *delimiter);
char	*handle_heredoc_delimiter(char *delimiter);
char	*get_next_line(int fd);
int     execute_normal_command(command_t *cmd); 

#endif 