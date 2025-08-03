/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:19:04 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/03 15:02:45 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_expand(char status)
{
	if (status == '\'')
		return (0);
	return (1);
}

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

static void	init_expand_data(t_expand_data *data, char *arg, int dollar_pos)
{
	data->before = ft_substr(arg, 0, dollar_pos);
	data->after = NULL;
	data->result = NULL;
	data->new_pos = 0;
}

static void	set_after_part(t_expand_data *data, char *arg, int var_end)
{
	data->after = ft_strdup(&arg[var_end]);
}

static void	create_result(t_expand_data *data, char *env_value)
{
	int	env_len;

	if (!env_value)
		data->result = ft_strjoin(data->before, data->after);
	else
		data->result = ft_strjoin_three(data->before, env_value, data->after);
	env_len = 0;
	if (env_value)
		env_len = ft_strlen(env_value);
	data->new_pos = ft_strlen(data->before) + env_len;
}

static void	cleanup_expand_data(t_expand_data *data)
{
	if (data->before)
		free(data->before);
	if (data->after)
		free(data->after);
}

static char	*expand_variable_parts(char *arg, char *env_value, int dollar_pos, int var_end)
{
	t_expand_data	data;

	init_expand_data(&data, arg, dollar_pos);
	set_after_part(&data, arg, var_end);
	create_result(&data, env_value);
	cleanup_expand_data(&data);
	return (data.result);
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

static char	*expand_all_variables(char *arg, t_env **env_list_ptr)
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

char	*expand_or_not(parser_t *parser, char status)
{
	char	*current_arg;
	char	*expanded;
	t_env	**env_list_ptr;

	env_list_ptr = init_env(NULL);
	if (!parser || !parser->current || !parser->current->value)
		return (NULL);
	current_arg = parser->current->value;
	if (!should_expand(status))
		return (ft_strdup(current_arg));
	if (!env_list_ptr)
		return (ft_strdup(current_arg));
	expanded = expand_all_variables(current_arg, env_list_ptr);
	if (!expanded)
		return (ft_strdup(current_arg));
	return (expanded);
}
