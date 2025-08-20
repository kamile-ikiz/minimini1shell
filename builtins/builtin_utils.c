/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:41:38 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/20 17:57:01 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	if (ft_isdigit(*str) || *str == '=')
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_env	*find_env_var(t_env *list, char *key)
{
	while (list)
	{
		if (ft_strcmp(list->key, key) == 0)
			return (list);
		list = list->next;
	}
	return (NULL);
}

int	print_identifier_error(char *identifier)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(identifier, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	set_exit_code(1);
	return (1);
}

int	env_list_size(t_env *list)
{
	int	count;

	count = 0;
	while (list)
	{
		count++;
		list = list->next;
	}
	return (count);
}

void	reset_print_flags(t_env *list)
{
	while (list)
	{
		list->is_printed = false;
		list = list->next;
	}
}
