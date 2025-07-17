/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/15 19:53:45 by kikiz            ###   ########.fr       */
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
char *parse_quotes(parser_t *parser, char quote)
{
	int	start = ++parser->pos;
	int	len = 0;
	while(parser->inp[parser->pos] && parser->inp[parser->pos] != quote)
	{
		if(parser->inp[parser->pos] == '\\' && parser->inp[parser->pos + 1])
			parser->pos++;
		parser->pos++;
		len ++;
	}
	if(!parser->inp[parser->pos])
	{
		parser->error = 1;
		parser->error_msg = "Unclosed quote";
		return NULL;
	}
	char *res = ft_substr(parser->inp, start, parser->pos - start);
	parser->pos++;
	return(res);
}

// Parse quoted string
char *parse_quoted_string(parser_t *parser, char quote) {
    int start = ++parser->pos; // Skip opening quote
    int len = 0;
    
    while (parser->inp[parser->pos] && parser->inp[parser->pos] != quote) {
        if (parser->inp[parser->pos] == '\\' && parser->inp[parser->pos + 1]) {
            parser->pos++; // Skip escape character
        }
        parser->pos++;
        len++;
    }
    
    if (!parser->inp[parser->pos]) {
        parser->error = 1;
        parser->error_msg = "Unclosed quote";
        return NULL;
    }
    
    char *result = ft_substr(parser->inp, start, parser->pos - start);
    parser->pos++; // Skip closing quote
    return result;
}

char *parse_word(parser_t *parser) {
    int start = parser->pos;
    
    while (parser->inp[parser->pos] && 
           !isspace(parser->inp[parser->pos]) &&
           parser->inp[parser->pos] != '|' &&
           parser->inp[parser->pos] != '<' &&
           parser->inp[parser->pos] != '>' &&
           parser->inp[parser->pos] != '&' &&
           parser->inp[parser->pos] != ';' &&
           parser->inp[parser->pos] != '(' &&
           parser->inp[parser->pos] != ')' &&
           parser->inp[parser->pos] != '"' &&
           parser->inp[parser->pos] != '\'') {
        parser->pos++;
    }
    
    if (parser->pos == start) return NULL;
    return ft_substr(parser->inp, start, parser->pos - start);
}

//tokenize input string
token_t	*tokenize(char *input)
{
	parser_t parser = {0};
	parser.inp = input;
	parser.pos = 0;
	token_t *tokens = NULL;

	while(parser.inp[parser.pos])
	{
		skip_whitespace(&parser);
		if(!parser.inp[parser.pos])
			break;
		char c = parser.inp[parser.pos];
		token_t *token = NULL;
		if(c == '|' && parser.inp[parser.pos] != '|')
		{
			token = new_token(TOKEN_PIPE, "|");
			parser.pos ++;
		}
		else if (c == '<')
		{
			if(parser.inp[parser.pos + 1] == '<')
			{
				token = new_token(TOKEN_HEREDOC, "<<");
				parser.pos += 2;
			}
			else
			{
				token = new_token(TOKEN_REDIRECT_IN, "<");
				parser.pos ++;
			}
		}
		else if (c == '>')
		{
			if(parser.inp[parser.pos + 1] == '>')
			{
				token = new_token(TOKEN_REDIRECT_APPEND, ">>");
				parser.pos += 2;
			}
			else
			{
				token = new_token(TOKEN_REDIRECT_OUT, ">");
				parser.pos++;
			}
		}
		else if(c == ';')
		{
			token = new_token(TOKEN_SEMICOLON, ";");
			parser.pos ++;
		}
		else if(c == '(')
		{
			token = new_token(TOKEN_LPAREN, "(");
			parser.pos ++;
		}
		else if(c == ')')
		{
			token = new_token(TOKEN_RPAREN, ")");
			parser.pos ++;
		}
		else if(c == '\'')
		{
			char *quoted = parse_quoted_string(&parser, c);
			if(quoted)
			{
				token = new_token(TOKEN_WORD, quoted);
				free(quoted);
			}
			else
				token = new_token(TOKEN_ERROR, "quote_error");
		}
		else
		{
			char *word = parse_word(&parser);
			if(word)
			{
				token = new_token(TOKEN_WORD, word);
				free(word);
			}
			else
				parser.pos++;
		}
		if (token)
			token_lst(&tokens, token);
		if(parser.error)
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
		

}
