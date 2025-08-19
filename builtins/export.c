/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 01:40:35 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/19 15:36:35 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	print_exported_node(t_env *node)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(node->key, STDOUT_FILENO);
	if (node->value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(node->value, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	ft_putstr_fd("\n", STDOUT_FILENO);
	node->is_printed = true;
}

static t_env	*find_and_mark_next_smallest(t_env *list)
{
	t_env	*current;
	t_env	*smallest;

	current = list;
	smallest = NULL;
	while (current)
	{
		if (!current->is_printed && current->is_exported
			&& (!smallest || ft_strcmp(current->key, smallest->key) < 0))
			smallest = current;
		current = current->next;
	}
	if (smallest)
		smallest->is_printed = true;
	return (smallest);
}

static int	print_exported_vars(t_env *list)
{
	t_env	*smallest;
	t_env	*current;
	int		printed_count;
	int		total_vars;

	total_vars = env_list_size(list);
	printed_count = 0;
	while (printed_count < total_vars)
	{
		smallest = find_and_mark_next_smallest(list);
		if (!smallest)
			break ;
		print_exported_node(smallest);
		printed_count++;
	}
	current = list;
	while (current)
	{
		current->is_printed = false;
		current = current->next;
	}
	set_exit_code(0); // Başarılı durumda exit code 0
	return (0);
}

int	builtin_export(t_command *cmd)
{
	t_env	**list_ptr;
	t_env	*list;
	int		i;
	char	*eq_pos;
	int		error_flag;

	list_ptr = init_env(NULL);
	list = *list_ptr;
	error_flag = 0;
	if (!cmd->args[1])
		return (print_exported_vars(list));
	i = 0;
	while (cmd->args[++i])
	{
		if (!is_valid_identifier(cmd->args[i]))
		{
			print_identifier_error(cmd->args[i]);
			error_flag = 1; // Hatalı identifier bulundu
			continue ;
		}
		eq_pos = ft_strchr(cmd->args[i], '=');
		if (eq_pos)
			handle_var_with_value(list_ptr, cmd->args[i], eq_pos);
		else
			handle_var_only(list_ptr, cmd->args[i]);
	}
	if (error_flag)
		set_exit_code(1);
	else
		set_exit_code(0);
	return (error_flag);
}
