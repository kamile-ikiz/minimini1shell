/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:05:03 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/12 17:23:33 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect_token(token_t token)
{
	token_type_t type;
	type = token.type;
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_REDIRECT_APPEND || type == TOKEN_HEREDOC)
		return(1);
	else
		return(0);
}

static	redirect_t	*create_redirect(token_type_t type, char *filename)
{
	redirect_t	*redirect;

	redirect = malloc(sizeof(redirect_t));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->filename = ft_strdup(filename);
	if (!redirect->filename)
	{
		free(redirect);
		return (NULL);
	}
	redirect->next = NULL;
	return (redirect);
}

static	void	add_redirect(redirect_t **head, redirect_t *new_redirect)
{
	redirect_t	*current;

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

int	handle_redirect_pair(token_t *redirect_token, token_t *filename,
	command_t *cmd)
{
	redirect_t	*new_redirect;
	char *file_name;

	if (!cmd || !redirect_token || !filename)
		return (-1);
	file_name = filename->value;
	new_redirect = create_redirect(redirect_token->type, file_name);
	if (!new_redirect)
		return (-1);
	add_redirect(&cmd->redirects, new_redirect);
	return (0);
}
