NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I. -Ilibft
LDFLAGS = -lreadline -no-pie

SRCDIR = .
OBJDIR = obj
BUILTINSDIR = builtins
EXECUTEDIR = execute
EXTRASDIR = extras
LIBFTDIR = libft
TOKEN_PARSEDIR = token_parse

SRCS =	main.c

BUILTINS_SRCS = ./builtins/builtin_utils_ex.c \
				./builtins/builtin_utils.c \
				./builtins/cd.c \
				./builtins/echo.c \
				./builtins/env.c \
				./builtins/exit.c \
				./builtins/export.c \
				./builtins/is_builtin.c \
				./builtins/pwd.c \
				./builtins/unset.c

EXECUTE_SRCS = 	./execute/execute_builtin.c \
				./execute/execute_pipeline.c \
				./execute/execute_simple.c \
				./execute/execute_utils.c \
				./execute/executor.c \
				./execute/execve_command.c \
				./execute/pipeline_child.c \
				./execute/t_env_to_char_p.c

EXTRAS_SRCS = 	./extras/exit_code.c \
				./extras/free_ex.c \
				./extras/free.c \
				./extras/heredoc_expand.c \
				./extras/heredoc_process.c \
				./extras/heredoc.c \
				./extras/redirect_handle.c \
				./extras/redirect_operations.c \
				./extras/signal_execution.c \
				./extras/signal_heredoc.c \
				./extras/signal_prompt.c 

TOKEN_PARSE_SRCS =	./token_parse/check_syntax_utils.c \
					./token_parse/check_syntax.c \
					./token_parse/create_command.c \
					./token_parse/create_redirect.c \
					./token_parse/expansion_utils_ex.c \
					./token_parse/expansion_utils.c \
					./token_parse/expansion.c \
					./token_parse/handle_line.c \
					./token_parse/parser_utils.c \
					./token_parse/parser.c \
					./token_parse/process_input_line.c \
					./token_parse/segment_utils_ex.c \
					./token_parse/segment_utils.c \
					./token_parse/segment.c \
					./token_parse/token_utils_ex.c \
					./token_parse/token_utils.c \
					./token_parse/token.c \
					./token_parse/utils.c


ALL_SRCS = $(SRCS) $(BUILTINS_SRCS) $(EXECUTE_SRCS) $(EXTRAS_SRCS) $(TOKEN_PARSE_SRCS)
OBJS = $(ALL_SRCS:%.c=$(OBJDIR)/%.o)

LIBFT = $(LIBFTDIR)/libft.a

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m
BOLD = \033[1m

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(BOLD)$(GREEN)🔗 Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)
	@echo "$(BOLD)$(GREEN)✅ $(NAME) compiled successfully!$(NC)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@echo "$(BOLD)$(YELLOW)📚 Building libft...$(NC)"
	@make -C $(LIBFTDIR) --no-print-directory

clean:
	@echo "$(RED)🧹 Cleaning object files...$(NC)"
	@rm -rf $(OBJDIR)
	@make -C $(LIBFTDIR) clean

fclean: clean
	@echo "$(RED)🗑️ Removing $(NAME)...$(NC)"
	@rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re