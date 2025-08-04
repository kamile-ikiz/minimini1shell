#include "minishell.h"

// ANSI Color Codes
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

// Unicode symbols for better visual appeal
#define PIPE_SYMBOL     "┃"
#define ARROW_RIGHT     "→"
#define ARROW_LEFT      "←"
#define DOUBLE_ARROW    "⇉"
#define PAREN_OPEN      "❰"
#define PAREN_CLOSE     "❱"
#define EOF_SYMBOL      "⌘"
#define ERROR_SYMBOL    "⚠"

static const char *get_token_color(token_type_t type)
{
	switch (type)
	{
		case TOKEN_WORD:           return GREEN;
		case TOKEN_PIPE:           return YELLOW;
		case TOKEN_REDIRECT_IN:    return BLUE;
		case TOKEN_REDIRECT_OUT:   return CYAN;
		case TOKEN_REDIRECT_APPEND: return MAGENTA;
		case TOKEN_HEREDOC:        return BLUE;
		case TOKEN_ERROR:          return RED;
		default:                   return RESET_COLOR;
	}
}

static const char *get_token_symbol(token_type_t type)
{
	switch (type)
	{
		case TOKEN_WORD:           return "📝";
		case TOKEN_PIPE:           return PIPE_SYMBOL;
		case TOKEN_REDIRECT_IN:    return ARROW_LEFT;
		case TOKEN_REDIRECT_OUT:   return ARROW_RIGHT;
		case TOKEN_REDIRECT_APPEND: return DOUBLE_ARROW;
		case TOKEN_HEREDOC:        return "⟪";
		case TOKEN_ERROR:          return ERROR_SYMBOL;
		default:                   return "❓";
	}
}

static const char *get_token_type_name(token_type_t type)
{
	switch (type)
	{
		case TOKEN_WORD:           return "WORD";
		case TOKEN_PIPE:           return "PIPE";
		case TOKEN_REDIRECT_IN:    return "REDIR_IN";
		case TOKEN_REDIRECT_OUT:   return "REDIR_OUT";
		case TOKEN_REDIRECT_APPEND: return "REDIR_APPEND";
		case TOKEN_HEREDOC:        return "HEREDOC";
		case TOKEN_ERROR:          return "ERROR";
		default:                   return "UNKNOWN";
	}
}

void print_tokens_fancy(token_t *tokens)
{
	token_t *current = tokens;
	int token_count = 0;
	
	printf("\n%s╔═══════════════════════════════════════════════════════════════════════╗%s\n", CYAN, RESET_COLOR);
	printf("%s║%s                           %sTOKEN ANALYSIS%s                           %s║%s\n", 
		CYAN, RESET_COLOR, BOLD, RESET_COLOR, CYAN, RESET_COLOR);
	printf("%s╠═══════════════════════════════════════════════════════════════════════╣%s\n", CYAN, RESET_COLOR);
	
	while (current)
	{
		const char *color = get_token_color(current->type);
		const char *symbol = get_token_symbol(current->type);
		const char *type_name = get_token_type_name(current->type);
		
		printf("%s║%s %s%s%s %s%-12s%s", 
			CYAN, RESET_COLOR, color, symbol, RESET_COLOR, 
			color, type_name, RESET_COLOR);
		
		if (current->value && current->next != NULL)
		{
			printf(" %s→%s %s\"%s\"%s", 
				GRAY, RESET_COLOR, color, current->value, RESET_COLOR);
		}
		
		printf("%*s %s║%s\n", 
			(int)(45 - (current->value ? strlen(current->value) : 0) - strlen(type_name)), "",
			CYAN, RESET_COLOR);
		
		current = current->next;
		token_count++;
	}
	
	printf("%s╠═══════════════════════════════════════════════════════════════════════╣%s\n", CYAN, RESET_COLOR);
	printf("%s║%s                      %sTotal tokens: %d%s                         %s║%s\n", 
		CYAN, RESET_COLOR, BOLD, token_count, RESET_COLOR, CYAN, RESET_COLOR);
	printf("%s╚═══════════════════════════════════════════════════════════════════════╝%s\n", CYAN, RESET_COLOR);
}

// Simple version for those who prefer minimal output
void print_tokens_simple(token_t *tokens)
{
	token_t *current = tokens;
	int i = 0;
	
	printf("\n%sTokens:%s ", BOLD, RESET_COLOR);
	while (current)
	{
		const char *color = get_token_color(current->type);
		
		if (i > 0)
			printf(" %s→%s ", GRAY, RESET_COLOR);
			
		printf("%s[%s]%s", color, 
			current->value ? current->value : get_token_type_name(current->type), 
			RESET_COLOR);

		current = current->next;
		i++;
	}
	printf("\n");
}