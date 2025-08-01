/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:13:48 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/01 16:42:37 by kikiz            ###   ########.fr       */
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

static void	free_args(char **args, int argc)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (i < argc)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_command(command_t *cmd)
{
	if (!cmd)
		return ;
	free_args(cmd->args, cmd->argc);
	free_redirects(cmd->redirects);
	free(cmd);
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

int	add_word_to_command(command_t *cmd, const char *word)
{
	char	**new_args;

	if (!cmd || !word)
		return (-1);
	new_args = realloc_args(cmd->args, cmd->argc + 1);
	if (!new_args)
		return (-1);
	new_args[cmd->argc] = strdup(word);
	if (!new_args[cmd->argc])
		return (-1);
	cmd->args = new_args;
	cmd->argc++;
	return (0);
}