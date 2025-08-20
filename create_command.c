/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:13:48 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/20 17:48:58 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
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

int	handle_command_pair(t_token *word, t_command *cmd)
{
	char	**new_args;

	if (!cmd || !word)
		return (-1);
	if(word->value[0] == '\0' && word->was_quoted == 0)
		return (0);
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
