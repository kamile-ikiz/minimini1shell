/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils_ex.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:29:28 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 01:22:24 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *var_name, t_env **env_list_ptr)
{
	t_env	*current;

	if (!env_list_ptr || !*env_list_ptr || !var_name)
		return (NULL);
	current = *env_list_ptr;
	while (current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}
