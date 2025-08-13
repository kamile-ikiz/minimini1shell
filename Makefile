NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I. -Ilibft
LDFLAGS = -lreadline -no-pie

SRCDIR = .
OBJDIR = obj
LIBFTDIR = libft
BUILTINSDIR = builtins
HEREDOCDIR = heredoc

# Ana kaynak dosyalar
SRCS =	token_utils.c \
		token.c \
		parser.c \
		parser_utils.c \
		executor.c \
    	utils.c \
    	free.c \
		get_next_line.c \
    	check_syntax.c \
		check_syntax_utils.c \
		expansion.c \
		expansion_utils.c \
		create_command.c \
		create_redirect.c \
		segment.c \
		segment_utils.c \
		redirect_operations.c \
		signal.c \
		test.c \
		heredoc.c \
		heredoc_process.c \
		heredoc_expand.c

# builtins klasöründeki tüm .c dosyalarını ekle
BUILTINS_SRCS = $(wildcard $(BUILTINSDIR)/*.c)
HEREDOC_SRCS = $(wildcard $(HEREDOCDIR)/*.c)

ALL_SRCS = $(SRCS) $(BUILTINS_SRCS) $(HEREDOC_SRCS)
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

leak:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind.supp ./minishell

.PHONY: all clean fclean re