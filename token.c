/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/20 21:29:04 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

token_t	*new_token(token_type_t type, char *value)
{
	token_t *token = malloc(sizeof(token_t));
	if(!token)
		return(NULL);
	token->type = type;
	if(value != NULL)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

void	token_lst(token_t **head, token_t *token)
{
	if(!*head)
		*head = token;
	else
	{
		token_t *current = *head;
		while(current->next)
			current = current->next;
		current->next = token;
	}
}
//INPUT TOKENIZE

static void	init_parser(parser_t *parser, char *input)
{
	parser->inp = input;
	parser->pos = 0;
	parser->error = 0;
}

static token_t	*handle_operator_tokens(parser_t *parser)
{
	char	c;
	c = parser->inp[parser->pos];

	if (c == '|' && parser->inp[parser->pos + 1] != '|')
		return (parser->pos++, new_token(TOKEN_PIPE, "|"));
	if (c == '<')
	{
		if (parser->inp[parser->pos + 1] == '<')
			return (parser->pos += 2, new_token(TOKEN_HEREDOC, "<<"));
		return (parser->pos++, new_token(TOKEN_REDIRECT_IN, "<"));
	}
	if (c == '>')
	{
		if (parser->inp[parser->pos + 1] == '>')
			return (parser->pos += 2, new_token(TOKEN_REDIRECT_APPEND, ">>"));
		return (parser->pos++, new_token(TOKEN_REDIRECT_OUT, ">"));
	}
	if (c == '(')
		return (parser->pos++, new_token(TOKEN_LPAREN, "("));
	if (c == ')')
		return (parser->pos++, new_token(TOKEN_RPAREN, ")"));
	return (NULL);
}

static token_t	*handle_quoted_or_word(parser_t *parser)
{
	char	*val;
	token_t	*token;

	if (parser->inp[parser->pos] == '\'')
	{
		val = parse_quoted_string(parser, '\'');
		if (!val)
			return (new_token(TOKEN_ERROR, "quote_error"));
		token = new_token(TOKEN_WORD, val);
		free(val);
		return (token);
	}
	val = parse_word(parser);
	if (!val)
		return (NULL);
	token = new_token(TOKEN_WORD, val);
	free(val);
	return (token);
}

static int	process_token(parser_t *parser, token_t **tokens)
{
	token_t *token;

	skip_whitespace(parser);
	if (!parser->inp[parser->pos])
		return (0);
	token = handle_operator_tokens(parser);
	if (!token)
		token = handle_quoted_or_word(parser);
	if (token)
		token_lst(tokens, token);
	if (parser->error)
	{
		free_tokens(*tokens);
		return (-1);
	}
	return (1);
}

token_t	*tokenize(char *input)
{
	parser_t	parser = {0};
	token_t		*tokens;
	tokens = NULL;
	
	int			status;

	init_parser(&parser, input);
	while (parser.inp[parser.pos])
	{
		status = process_token(&parser, &tokens);
		if (status == -1)
			return (NULL);
		else if (status == 0)
			break ;
	}
	token_lst(&tokens, new_token(TOKEN_EOF, NULL));
	return (tokens);
}

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
		case TOKEN_LPAREN:         return WHITE;
		case TOKEN_RPAREN:         return WHITE;
		case TOKEN_EOF:            return GRAY;
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
		case TOKEN_LPAREN:         return PAREN_OPEN;
		case TOKEN_RPAREN:         return PAREN_CLOSE;
		case TOKEN_EOF:            return EOF_SYMBOL;
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
		case TOKEN_LPAREN:         return "LPAREN";
		case TOKEN_RPAREN:         return "RPAREN";
		case TOKEN_EOF:            return "EOF";
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
		
		if (current->value && current->type != TOKEN_EOF)
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
