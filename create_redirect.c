/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:05:03 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/24 06:04:25 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect_token(t_token token)
{
	t_token_type	type;

	type = token.type;
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_HEREDOC)
		return (1);
	else
		return (0);
}

static	t_redirect	*create_redirect(t_token_type type, char *filename)
{
	t_redirect	*redirect;

	redirect = malloc(sizeof(t_redirect));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->filename = ft_strdup(filename);
	redirect->heredoc_pipe_fd = 0;
	redirect->next = NULL;
	return (redirect);
}

static	void	add_redirect(t_redirect **head, t_redirect *new_redirect)
{
	t_redirect	*current;

	if (!*head)
	{
		*head = new_redirect;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_redirect;
}

int	handle_redirect_pair(t_token *redirect_token, t_token *filename,
	t_command *cmd)
{
	t_redirect	*new_redirect;
	char		*file_name;

	if (!cmd || !redirect_token || !filename)
		return (-1);
	file_name = filename->value;
	new_redirect = create_redirect(redirect_token->type, file_name);
	if (!new_redirect)
		return (-1);
	add_redirect(&cmd->redirects, new_redirect);
	return (0);
}
