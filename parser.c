/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:52:51 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/06 21:53:23 by kikiz            ###   ########.fr       */
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

// Pipeline'a yeni bir komut ekleyen yardımcı fonksiyon
static void	add_command_to_pipeline(command_t **pipeline_head, command_t *new_cmd)
{
	command_t	*current;

	if (!*pipeline_head)
	{
		*pipeline_head = new_cmd;
		return ;
	}
	current = *pipeline_head;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

/*
** create_pipeline_from_segments: Segment listesini alır ve bunu
** command_t yapılarından oluşan bir pipeline'a dönüştürür.
** Hata durumunda NULL döner ve o ana kadar oluşturulan pipeline'ı temizler.
*/
static command_t	*create_pipeline_from_segments(segment_t *segments)
{
	command_t	*pipeline_head;
	command_t	*new_command;
	segment_t	*current_segment;

	pipeline_head = NULL;
	current_segment = segments;
	while (current_segment)
	{
		new_command = NULL;
		if (parse_command_or_redirect(current_segment, &new_command) == -1)
		{
			// Bir segmentin ayrıştırılması başarısız olursa, tüm süreci iptal et.
			free_pipeline(pipeline_head);
			return (NULL);
		}
		add_command_to_pipeline(&pipeline_head, new_command);
		current_segment = current_segment->next;
	}
	return (pipeline_head);
}

static token_t	*lex_and_validate(char *input)
{
	token_t	*tokens;

	tokens = tokenize(input);
	if (!tokens)
		return (NULL);
	if (!check_all_syntax(tokens))
	{
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}

/*
** parse_input: Ana ayrıştırıcı fonksiyon.
** Süreci yönetir: lexing, validation, segmentation ve pipeline oluşturma.
** Sonuç olarak çalıştırılmaya hazır pipeline'ı veya hata durumunda NULL döndürür.
*/
command_t	*parse_input(char *input)
{
	token_t		*tokens;
	segment_t	*segments;
	command_t	*pipeline_head;

	tokens = lex_and_validate(input);
	if (!tokens)
		return (NULL);
	segments = split_tokens_by_pipe(tokens);
	free_tokens(tokens);
	if (!segments)
		return (NULL);
	pipeline_head = create_pipeline_from_segments(segments);
	free_segments(segments); 
	return (pipeline_head);
}