/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:05:03 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/01 16:43:34 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

redirect_t	*create_redirect(token_type_t type, const char *filename)
{
	redirect_t	*redirect;

	redirect = malloc(sizeof(redirect_t));
	if (!redirect)
		return (NULL);
	redirect->type = type;
	redirect->filename = NULL;
	if (filename)
		redirect->filename = strdup(filename);
	redirect->next = NULL;
	return (redirect);
}

void	add_redirect(redirect_t **head, redirect_t *new_redirect)
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

void	free_redirects(redirect_t *redirects)
{
	redirect_t	*temp;

	while (redirects)
	{
		temp = redirects;
		redirects = redirects->next;
		free(temp->filename);
		free(temp);
	}
}