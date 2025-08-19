/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:19:04 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/19 22:37:48 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(char *str, int start, int *end)
{
	int		i;
	char	*var_name;

	i = start;
	if (str[i] == '$')
		i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*end = i;
	var_name = ft_substr(str, start + 1, i - start - 1);
	return (var_name);
}

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


static char	*expand_single_variable(char *arg, int dollar_pos, t_env **env_list_ptr, int *new_i)
{
	char	*var_name;
	char	*env_value;
	char	*result;
	int		var_end;

	var_name = extract_var_name(arg, dollar_pos, &var_end);
	if (!var_name)
		return (ft_strdup(arg));
	env_value = get_env_value(var_name, env_list_ptr);
	result = expand_variable_parts(arg, env_value, dollar_pos, var_end);
	if (env_value)
		*new_i = dollar_pos + ft_strlen(env_value);
	else
		*new_i = dollar_pos;
	free(var_name);
	return (result);
}

static int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

char	*expand_all_variables(char *arg, t_env **env_list_ptr)
{
	char	*result;
	char	*temp;
	int		i;
	int		new_i;

	if (!arg || !ft_strchr(arg, '$'))
		return (ft_strdup(arg));
	result = ft_strdup(arg);
	if (!result)
		return (NULL);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && is_valid_var_char(result[i + 1]))
		{
			temp = expand_single_variable(result, i, env_list_ptr, &new_i);
			free(result);
			result = temp;
			i = new_i;
		}
		else if (result[i] == '$' && result[i + 1] == '?')
			{
				temp = ft_itoa(get_exit_code());
				free(result);
				result = temp;
				i += 2;
			}
		else
			i++;
	}
	return (result);
}
