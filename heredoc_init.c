/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:13:44 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/31 16:33:11 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
 //execute_normal_command YAZILACAK
int	execute_command_with_heredoc(command_t *cmd)
{
	int	original_stdin;
	int	status;

	if (!cmd->heredoc_delimiter)
		return execute_normal_command(cmd); //execute_normal_command yazılacak
	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
		return (-1);
	if (setup_heredoc_redirect(cmd, cmd->heredoc_delimiter) == -1)
	{
		close(original_stdin);
		return (-1);
	}
	status = execute_normal_command(cmd);
	if (dup2(original_stdin, STDIN_FILENO) == -1)
	{
		close(original_stdin);
		return (-1);
	}
	close(original_stdin);
	return (status);
}
