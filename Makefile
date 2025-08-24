NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I. -Ilibft
LDFLAGS = -lreadline -no-pie

SRCDIR = .
OBJDIR = obj
LIBFTDIR = libft
BUILTINSDIR = builtins

# Ana kaynak dosyalar
SRCS =	token_utils.c \
		token_utils1.c \
		token.c \
		parser.c \
		parser_utils.c \
		executor.c \
    	utils.c \
    	free.c \
		free1.c \
    	check_syntax.c \
		check_syntax_utils.c \
		expansion.c \
		expansion_utils.c \
		expansion_utils1.c \
		create_command.c \
		create_redirect.c \
		segment.c \
		segment_utils.c \
		segment_utils1.c \
		redirect_operations.c \
		redirect_handle.c \
		signal_execution.c \
		signal_prompt.c \
		signal_heredoc.c \
		test.c \
		heredoc.c \
		heredoc_expand.c \
		heredoc_process.c \
		exit_code.c

# builtins klasöründeki tüm .c dosyalarını ekle
BUILTINS_SRCS = ./builtins/builtin_utils2.c ./builtins/builtin_utils.c ./builtins/cd.c ./builtins/echo.c ./builtins/env.c ./builtins/exit.c ./builtins/export.c ./builtins/is_builtin.c ./builtins/pwd.c ./builtins/unset.c

ALL_SRCS = $(SRCS) $(BUILTINS_SRCS)
OBJS = $(ALL_SRCS:%.c=$(OBJDIR)/%.o)

LIBFT = $(LIBFTDIR)/libft.a

GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m
BOLD = \033[1m

# ...existing code...

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(BOLD)$(GREEN)🔗 Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LDFLAGS)
	@echo "$(BOLD)$(GREEN)✅ $(NAME) compiled successfully!$(NC)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)🔨 Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: $(BUILTINSDIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)🔨 Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@echo "$(BOLD)$(YELLOW)📚 Building libft...$(NC)"
	@make -C $(LIBFTDIR)

clean:
	@echo "$(RED)🧹 Cleaning object files...$(NC)"
	@rm -rf $(OBJDIR)
	@make -C $(LIBFTDIR) clean

fclean: clean
	@echo "$(RED)🗑️  Removing $(NAME)...$(NC)"
	@rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re