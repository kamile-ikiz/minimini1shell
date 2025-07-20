/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:16:14 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/20 21:29:04 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
char *parse_quoted_string(parser_t *parser, char quote)
{
    int start = ++parser->pos; // Skip opening quote
    int len = 0;
    
    while (parser->inp[parser->pos] && parser->inp[parser->pos] != quote)
	{
        if (parser->inp[parser->pos] == '\\' && parser->inp[parser->pos + 1])
            parser->pos++; // Skip escape character
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

char *parse_word(parser_t *parser)
{
    int start;
    start = parser->pos;
    
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

//new command
command_t *new_command(void)
{
    command_t *cmd = malloc(sizeof(command_t));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->argc = 0;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_mode = 0;
    cmd->heredoc_delimiter = NULL;
    cmd->next = NULL;
    return cmd;
}

void add_argmnt(command_t *cmd, char *arg)
{
    if (!cmd || !arg)
        return;
    cmd->args = realloc(cmd->args, sizeof(char*) * (cmd->argc + 2));
    if (!cmd->args)
        return;
    cmd->args[cmd->argc] = ft_strdup(arg);
    cmd->args[cmd->argc + 1] = NULL;
    cmd->argc++;
}

command_t *parse_command(parser_t *parser)
{
    command_t *cmd = new_command();
    if (!cmd)
        return (NULL);
    while(parser->current &&
    parser->current->type != TOKEN_PIPE &&
    parser->current->type != TOKEN_EOF)
    {
        token_t *token = parser->current;
        if (token->type == TOKEN_WORD)
        {
            add_argmnt(cmd, token->value);
            parser->current = parser->current->next;
        }
        else if (token->type == TOKEN_REDIRECT_IN)
        {
            parser->current = parser->current->next;
            if (parser->current && parser->current->type == TOKEN_WORD)
            {
                cmd->input_file = ft_strdup(parser->current->value);
                parser->current = parser->current->next;
            }
            else
            {
                cmd->input_file = NULL;
                parser->error = 1;
                parser->error_msg = "Missing input file for <";
            }
        }
        else if (token->type == TOKEN_REDIRECT_OUT)
        {
            cmd->append_mode = 0;
            parser->current = parser->current->next;
            if (parser->current && parser->current->type == TOKEN_WORD)
            {
                cmd->output_file = ft_strdup(parser->current->value);
                parser->current = parser->current->next;
            }
            else
            {
                cmd->output_file = NULL;
                parser->error = 1;
                parser->error_msg = "Missing output file for >";
            }
        }
        else if (token->type == TOKEN_REDIRECT_APPEND)
        {
            cmd->append_mode = 1;
            parser->current = parser->current->next;
            if (parser->current && parser->current->type == TOKEN_WORD)
            {
                cmd->output_file = ft_strdup(parser->current->value);
                parser->current = parser->current->next;
            }
            else
            {
                cmd->output_file = NULL;
                parser->error = 1;
                parser->error_msg = "Missing output file for >>";
            }
        }
        else if (token->type == TOKEN_HEREDOC)
        {
            parser->current = parser->current->next;
            if (parser->current && parser->current->type == TOKEN_WORD)
            {
                cmd->heredoc_delimiter = ft_strdup(parser->current->value);
                // Here you would handle the heredoc logic
                // For now, we just store the delimiter
                parser->current = parser->current->next;
            }
            else
            {
                cmd->heredoc_delimiter = NULL;
                parser->error = 1;
            }
        }
    }

    return cmd;
}
