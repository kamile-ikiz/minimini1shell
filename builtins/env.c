/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 13:55:09 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 17:28:40 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*get_last_list(t_env *list)
{
	if (!list)
		return (NULL);
	while (list->next != NULL)
		list = list->next;
	return (list);
}

static void	parse_env(char *env_var, t_env **list)
{
	t_env	*new;
	int		i;

	if (!env_var)
		return ;
	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	i = 0;
	while (env_var[i] && env_var[i] != '=')
		i++;
	new->key = ft_substr(env_var, 0, i);
	if (env_var[i])
		new->value = ft_strdup(env_var + i + 1);
	else
		new->value = NULL;
	new->is_exported = true;
	new->next = NULL;
	if (!*list)
		*list = new;
	else
		get_last_list(*list)->next = new;
}

t_env	**init_env(char **env)
{
	static t_env	*list = NULL;
	int				i;

	if (list != NULL || !env)
		return (&list);
	i = 0;
	while (env[i])
	{
		parse_env(env[i], &list);
		i++;
	}
	return (&list);
}

static void	print_env_var(t_env *node)
{
	if (node->key && node->value && node->is_exported)
	{
		ft_putstr_fd(node->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(node->value, 1);
		ft_putstr_fd("\n", 1);
	}
}

int	builtin_env(t_command *cmd)
{
	t_env	**env;
	t_env	*current;

	if (cmd->args[1])
	{
		ft_putstr_fd("env: too many arguments\n", 2);
		set_exit_code(1);
		return (1);
	}
	env = init_env(NULL);
	if (!env || !*env)
	{
		set_exit_code(1);
		return (1);
	}
	current = *env;
	while (current)
	{
		print_env_var(current);
		current = current->next;
	}
	set_exit_code(0);
	return (0);
}
