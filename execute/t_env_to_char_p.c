/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_env_to_char_p.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 06:57:11 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/25 06:57:48 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_valid_envs(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		if (env_list->key && env_list->value)
			count++;
		env_list = env_list->next;
	}
	return (count);
}

static char	*create_env_string(t_env *node)
{
	char	*env_str;
	size_t	key_len;
	size_t	value_len;

	key_len = ft_strlen(node->key);
	value_len = ft_strlen(node->value);
	env_str = malloc(key_len + value_len + 2);
	if (!env_str)
		return (NULL);
	ft_memcpy(env_str, node->key, key_len);
	env_str[key_len] = '=';
	ft_memcpy(env_str + key_len + 1, node->value, value_len);
	env_str[key_len + value_len + 1] = '\0';
	return (env_str);
}

static int	populate_envp_array(char **envp, t_env *env_list)
{
	int	i;

	i = 0;
	while (env_list)
	{
		if (env_list->key && env_list->value)
		{
			envp[i] = create_env_string(env_list);
			if (!envp[i])
			{
				while (--i >= 0)
					free(envp[i]);
				return (-1);
			}
			i++;
		}
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (0);
}

char	**env_list_to_envp(t_env **env_list_ptr)
{
	char	**envp;
	int		count;

	if (!env_list_ptr || !*env_list_ptr)
		return (NULL);
	count = count_valid_envs(*env_list_ptr);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	if (populate_envp_array(envp, *env_list_ptr) == -1)
	{
		free(envp);
		return (NULL);
	}
	return (envp);
}
