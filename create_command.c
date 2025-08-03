/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:13:48 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/02 17:03:57 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

command_t	*create_command(void)
{
	command_t	*cmd;

	cmd = malloc(sizeof(command_t));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->argc = 0;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

static char	**realloc_args(char **args, int new_size)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (new_size + 1));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < new_size - 1 && args)
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[new_size] = NULL;
	if (args)
		free(args);
	return (new_args);
}

int	handle_command_pair(token_t *word, command_t *cmd)
{
	char	**new_args;

	if (!cmd || !word)
		return (-1);
	new_args = realloc_args(cmd->args, cmd->argc + 1);
	if (!new_args)
		return (-1);
	new_args[cmd->argc] = strdup(word->value);
	if (!new_args[cmd->argc])
		return (-1);
	cmd->args = new_args;
	cmd->argc++;
	return (0);
}
