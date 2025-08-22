/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:56:11 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/22 16:54:02 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_new_var(t_env **list, char *key, char *value, bool is_exported)
{
	t_env	*new;
	t_env	*last;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->is_exported = is_exported;
	new->is_printed = false;
	new->next = NULL;
	if (!*list)
		*list = new;
	else
	{
		last = get_last_list(*list);
		last->next = new;
	}
}

void	handle_var_only(t_env **list, char *key)
{
	t_env	*existing;

	existing = find_env_var(*list, key);
	if (existing)
		existing->is_exported = true;
	else
		add_new_var(list, key, NULL, true);
}

void	handle_var_with_value(t_env **list, char *arg, char *eq_pos)
{
	char	*key;
	char	*value;
	t_env	*existing;

	key = ft_substr(arg, 0, eq_pos - arg);
	value = ft_strdup(eq_pos + 1);
	existing = find_env_var(*list, key);
	if (existing)
	{
		free(existing->value);
		existing->value = value;
		existing->is_exported = true;
		free(key);
	}
	else
	{
		add_new_var(list, key, value, true);
		free(key);
		free(value);
	}
}
