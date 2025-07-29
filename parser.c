/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:16:14 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/29 18:44:11 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//parse quoted string
char *parse_quotes(parser_t *parser, char quote)
{
	int	start;
	char *res;

	start = ++parser->pos; //skip quote
	while(parser->inp[parser->pos] && parser->inp[parser->pos] != quote)
		parser->pos++;
	if(!parser->inp[parser->pos])
	{
		parser->error = 1;
		parser->error_msg = "Unclosed quote";
		return (NULL);
	}
	res = (ft_substr(parser->inp, start, parser->pos - start));
	parser->pos++; //skip quote
	return(res);
}

char	*parse_word(parser_t *parser)
{
	int		start;
	char	*a;

	start = parser->pos;
	while (parser->inp[parser->pos]
		&& !is_space(parser->inp[parser->pos])
		&& parser->inp[parser->pos] != '|'
		&& parser->inp[parser->pos] != '<'
		&& parser->inp[parser->pos] != '>'
		&& parser->inp[parser->pos] != '"'
		&& parser->inp[parser->pos] != '\'')
		parser->pos++;
	if (parser->pos == start)
		return (NULL);
	a = ft_substr(parser->inp, start, parser->pos - start);
	return (a);
}

//new command
command_t	*new_command(void)
{
	command_t *cmd = malloc(sizeof(command_t));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
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

static void	handle_word(parser_t *parser, command_t *cmd)
{
	add_argmnt(cmd, parser->current->value);
	parser->current = parser->current->next;
}

static void	handle_redirect_in(parser_t *parser, command_t *cmd)
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

static void	handle_redirect_out(parser_t *parser, command_t *cmd)
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

static void	handle_redirect_append(parser_t *parser, command_t *cmd)
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

static void	handle_heredoc(parser_t *parser, command_t *cmd)
{
	parser->current = parser->current->next;
	if (parser->current && parser->current->type == TOKEN_WORD)
	{
		cmd->heredoc_delimiter = ft_strdup(parser->current->value);
		parser->current = parser->current->next;
	}
	else
	{
		cmd->heredoc_delimiter = NULL;
		parser->error = 1;
	}
}

command_t	*parse_command(parser_t *parser)
{
	command_t	*cmd;
	token_t		*token;

	cmd = new_command();
	if (!cmd)
		return (NULL);
	while (parser->current &&
		parser->current->type != TOKEN_PIPE &&
		parser->current->type != TOKEN_EOF)
	{
		token = parser->current;
		if (token->type == TOKEN_WORD)
			handle_word(parser, cmd);
		else if (token->type == TOKEN_REDIRECT_IN)
			handle_redirect_in(parser, cmd);
		else if (token->type == TOKEN_REDIRECT_OUT)
			handle_redirect_out(parser, cmd);
		else if (token->type == TOKEN_REDIRECT_APPEND)
			handle_redirect_append(parser, cmd);
		else if (token->type == TOKEN_HEREDOC)
			handle_heredoc(parser, cmd);
	}
	return (cmd);
}
