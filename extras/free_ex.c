/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:34:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 14:00:35 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_environment(t_env **env_list)
{
	t_env	*current;
	t_env	*next;

	current = *env_list;
	while (current != NULL)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	*env_list = NULL;
}

void	free_heredoc(t_command *cmd)
{
	static t_command	*tmp;

	if (cmd)
		tmp = cmd;
	if (!cmd && tmp)
	{
		free_environment(init_env(NULL));
		free_commands(tmp);
	}
}
