/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:19:04 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 02:31:07 by kikiz            ###   ########.fr       */
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
	if (str[i++] == '?')
	{
		*end = i;
		var_name = ft_substr(str, start + 1, i - start - 1);
		return (var_name);
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*end = i;
	var_name = ft_substr(str, start + 1, i - start - 1);
	return (var_name);
}

static char	*handle_exit_code_var(t_expand_data *data)
{
	char	*exit_code_str;
	char	*result;

	exit_code_str = ft_itoa(get_exit_code());
	result = expand_variable_parts(data->arg, exit_code_str,
			data->dollar_pos, data->var_end);
	if (exit_code_str)
	{
		*(data->new_i) = data->dollar_pos + ft_strlen(exit_code_str);
		free(exit_code_str);
	}
	else
		*(data->new_i) = data->dollar_pos;
	return (result);
}

static char	*handle_regular_var(char *var_name, t_expand_data *data)
{
	char	*env_value;
	char	*result;

	env_value = get_env_value(var_name, data->env_list_ptr);
	result = expand_variable_parts(data->arg, env_value,
			data->dollar_pos, data->var_end);
	if (env_value)
		*(data->new_i) = data->dollar_pos + ft_strlen(env_value);
	else
		*(data->new_i) = data->dollar_pos;
	return (result);
}

char	*expand_single_variable(char *arg, int dollar_pos,
	t_env **env_list_ptr, int *new_i)
{
	t_expand_data	data;
	char			*var_name;
	char			*result;

	var_name = extract_var_name(arg, dollar_pos, &data.var_end);
	if (!var_name)
		return (ft_strdup(arg));
	data.arg = arg;
	data.env_list_ptr = env_list_ptr;
	data.dollar_pos = dollar_pos;
	data.new_i = new_i;
	if (var_name[0] == '?')
		result = handle_exit_code_var(&data);
	else
		result = handle_regular_var(var_name, &data);
	free(var_name);
	return (result);
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
		else
			i++;
	}
	return (result);
}
