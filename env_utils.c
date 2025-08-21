/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:42:50 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 03:17:18 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_vars(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->key && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*alloc_env_string(t_env *env)
{
	int		len;
	char	*str;

	len = ft_strlen(env->key) + ft_strlen(env->value) + 2;
	str = malloc(len);
	if (!str)
		return (NULL);
	ft_strcpy(str, env->key);
	ft_strcat(str, "=");
	ft_strcat(str, env->value);
	return (str);
}

static int	fill_envp_array(t_env *env, char **envp)
{
	int	i;

	i = 0;
	while (env)
	{
		if (env->key && env->value)
		{
			envp[i] = alloc_env_string(env);
			if (!envp[i])
				return (i);
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (-1);
}

static void	free_envp_partial(char **envp, int last)
{
	while (--last >= 0)
		free(envp[last]);
	free(envp);
}

char	**env_list_to_envp(t_env **env_list_ptr)
{
	char	**envp;
	int		count;
	int		err;

	if (!env_list_ptr || !*env_list_ptr)
		return (NULL);
	count = count_env_vars(*env_list_ptr);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	err = fill_envp_array(*env_list_ptr, envp);
	if (err >= 0)
	{
		free_envp_partial(envp, err);
		return (NULL);
	}
	return (envp);
}
