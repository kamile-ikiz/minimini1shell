/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:34:40 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 14:02:00 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "builtins/builtins.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <ctype.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define PROMPT          "\001\033[38;5;205m\002minimini1shell$ \001\033[0m\002"

extern volatile sig_atomic_t	g_signal_flag;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
	int				expand_mode;
	int				was_quoted;
}	t_token;

typedef struct s_segment
{
	t_token				*tokens;
	int					token_count;
	struct s_segment	*next;
}	t_segment;

typedef struct s_redirect
{
	t_token_type		type;
	char				*filename;
	int					heredoc_pipe_fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				**args;
	int					argc;
	t_redirect			*redirects;
	struct s_command	*head_command;
	struct s_command	*next;
}	t_command;

typedef struct s_parser
{
	t_token	*token_list;
	char	*inp;
	int		pos;
	int		error;
	char	*error_msg;
}	t_parser;

typedef struct s_expand_data
{
	char	*arg;
	t_env	**env_list_ptr;
	int		dollar_pos;
	int		var_end;
	int		*new_i;
}	t_expand_data;

typedef struct s_expanded_part
{
	char	*before;
	char	*after;
	char	*result;
	int		new_pos;
}	t_expanded_part;

typedef struct s_pipeline_data
{
	int			cmd_count;
	pid_t		*pids;
	int			**pipes;
	t_command	*head;
}	t_pipeline_data;

int			execute_builtin_command(t_command *cmd);
int			execute_pipeline(t_command *cmd);
int			execute_simple_command(t_command *cmd);
int			wait_for_children(pid_t *pids, int cmd_count);
int			execve_command(char **args);
int			count_commands(t_command *cmd);
int			execute_command(t_command *cmd);
void		setup_pipe_redirections(int **pipes, int cmd_index, int cmd_count);
void		close_all_pipes(int **pipes, int pipe_count);
void		cleanup_pipeline(pid_t *pids, int **pipes, int cmd_count);
void		pipeline_child_routine(t_command *cmd,
				t_pipeline_data *data, int i);
char		*find_command_path(char *cmd);
char		**env_list_to_envp(t_env **env_list_ptr);

int			get_exit_code(void);
int			read_heredoc_until_delimiter(const char *delimiter,
				int write_fd, int expand_vars);
int			prepare_heredocs(t_command *cmd_list);
int			handle_input_redirect(char *filename);
int			handle_output_redirect(char *filename);
int			handle_append_redirect(char *filename);
int			execute_redirects(t_command *cmd);
void		set_exit_code(int status);
void		free_environment(t_env **env_list);
void		free_heredoc(t_command *cmd);
void		free_tokens(t_token *tokens);
void		free_args(char **args);
void		free_redirects(t_redirect *redirects);
void		free_commands(t_command *cmd);
void		free_segments(t_segment *segments);
void		cleanup_heredoc_pipes(t_command *cmd_list);
void		configure_execution_signals(void);
void		configure_heredoc_signals(void);
void		restore_default_signals(void);
void		configure_prompt_signals(void);
char		*expand(const char *input);

int			get_token_count(t_token *head);
int			check_pipe_syntax(t_token *tokens, int token_count);
int			check_all_syntax(t_token *head);
int			handle_command_pair(t_token *word, t_command *cmd);
int			is_redirect_token(t_token token);
int			handle_redirect_pair(t_token *redirect_token, t_token *filename,
				t_command *cmd);
int			is_valid_var_char(char c);
int			handle_line(char *line);
int			parse_error(t_command **cmd_ptr);
int			process_input_line(char *line);
int			parse_word_or_redirect(t_segment *segment, t_command **cmd_ptr);
int			is_space(char c);
int			handle_quoted_segment(t_parser *parser, char **segment,
				t_token *last_token);
int			handle_unquoted_segment(t_parser *parser, char **segment,
				t_token *last_token);
int			append_segment(char **word_ptr, char *segment);
int			is_word_delimiter(char c);
int			is_word_delimiter(char c);
void		*set_parser_error(t_parser *parser, char *msg, void *to_free);
void		init_parser(t_parser *parser, char *input);
void		print_syntax_error(char *token);
void		skip_whitespace(t_parser *parser);
char		*get_env_value(char *var_name, t_env **env_list_ptr);
char		*expand_variable_parts(char *arg, char *env_value,
				int dollar_pos, int var_end);
char		*expand_all_variables(char *arg, t_env **env_list_ptr);
char		*parse_unquoted_segment(t_parser *parser);
char		*parse_quotes(t_parser *parser, char quote);
t_command	*create_command(void);
t_command	*parse_input(char *input);
t_segment	*split_tokens_by_pipe(t_token *token_list);
t_segment	*create_single_segment(t_token *start_token, int count);
t_token		*handle_word(t_parser *parser);
t_token		*new_token(t_token_type type, char *value);
t_token		*token_lst(t_token **head, t_token *token);
t_token		*get_context_token(t_parser *parser, bool *is_new);
t_token		*tokenize(char *input);

#endif 
