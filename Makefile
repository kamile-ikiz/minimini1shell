# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/18 00:10:17 by kikiz             #+#    #+#              #
#    Updated: 2025/07/20 21:29:01 by kikiz            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Program name
NAME = minishell

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = -I. -Ilibft
LDFLAGS = -lreadline

# Directories
SRCDIR = .
OBJDIR = obj
LIBFTDIR = libft

# Source files
SRCS = main.c \
       token.c \
       parser.c \
       executor.c \
       builtins.c \
       signals.c \
       utils.c \
       free.c

# Object files
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

# Libft
LIBFT = $(LIBFTDIR)/libft.a

# Colors for pretty output
GREEN = \033[0;32m
YELLOW = \033[0;33m
RED = \033[0;31m
NC = \033[0m # No Color
BOLD = \033[1m

# Rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(BOLD)$(GREEN)🔗 Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	@echo "$(BOLD)$(GREEN)✅ $(NAME) compiled successfully!$(NC)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
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

# Test targets
test: $(NAME)
	@echo "$(BOLD)$(GREEN)🧪 Testing tokenizer...$(NC)"
	@./$(NAME) "ls -la | grep test > output.txt"
	@./$(NAME) "echo hello | cat << EOF"
	@./$(NAME) "(cd /tmp && ls) | wc -l"

debug: CFLAGS += -DDEBUG -fsanitize=address
debug: $(NAME)

# Development helpers
norm:
	@echo "$(BOLD)$(YELLOW)📝 Running norminette...$(NC)"
	@norminette $(SRCS) minishell.h

git-add:
	@git add .
	@echo "$(GREEN)✅ All files added to git$(NC)"

git-status:
	@git status --short

# Help
help:
	@echo "$(BOLD)Available targets:$(NC)"
	@echo "  $(GREEN)all$(NC)     - Build the project"
	@echo "  $(GREEN)clean$(NC)   - Remove object files"
	@echo "  $(GREEN)fclean$(NC)  - Remove object files and executable"
	@echo "  $(GREEN)re$(NC)      - Rebuild everything"
	@echo "  $(GREEN)test$(NC)    - Run basic tests"
	@echo "  $(GREEN)debug$(NC)   - Build with debug flags"
	@echo "  $(GREEN)norm$(NC)    - Check norm compliance"
	@echo "  $(GREEN)help$(NC)    - Show this help"

.PHONY: all clean fclean re test debug norm git-add git-status help

