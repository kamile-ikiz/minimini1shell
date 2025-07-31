/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:19:04 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/31 18:11:29 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int should_expand(char status)
{
    if (status == '\'')
        return (0);
    return (1);
}

static char *extract_var_name(char *str, int start, int *end)
{
    int i;
    char *var_name;

	i = start;
	if (str[i] == '$')
		i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	*end = i;
	var_name = ft_substr(str, start + 1, i -start -1);
	return (var_name);

}

char *get_env_value(char *var_name, t_env **env_list_ptr)
{
	t_env *current;

	if (!env_list_ptr || !*env_list_ptr || !var_name)
		return (NULL);
	current = *env_list_ptr;
	while(current)
	{
		if (ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current =current->next;
	}
	return (NULL);
}

static char	*expand_variable_parts(
	char *arg, char *env_value, int dollar_pos, int var_end, int *new_i)
{
	char	*before;
	char	*after;
	char	*result;
	int		env_len;

	before = ft_substr(arg, 0, dollar_pos);
	after = ft_strdup(&arg[var_end]);
	if (!env_value)
		result = ft_strjoin(before, after);
	else
		result = ft_strjoin_three(before, env_value, after);
	env_len = env_value ? ft_strlen(env_value) : 0;
	*new_i = ft_strlen(before) + env_len;
	free(before);
	free(after);
	return (result);
}

static char	*expand_single_variable(
	char *arg, int dollar_pos, t_env **env_list_ptr, int *new_i)
{
	char	*var_name;
	char	*env_value;
	char	*result;
	int		var_end;

	var_name = extract_var_name(arg, dollar_pos, &var_end);
	if (!var_name)
		return (ft_strdup(arg));
	env_value = get_env_value(var_name, env_list_ptr);
	result = expand_variable_parts(arg, env_value, dollar_pos, var_end, new_i);
	free(var_name);
	return (result);
}

static char	*expand_all_variables(char *arg, t_env **env_list_ptr)
{
	char	*result;
	char	*temp;
	int		i;
	int new_i;

	if (!arg || !ft_strchr(arg, '$'))
		return (ft_strdup(arg));
	result = ft_strdup(arg);
	if (!result)
		return (NULL);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && (ft_isalnum(result[i + 1]) || result[i + 1] == '_'))
		{
			temp = expand_single_variable(result, i, env_list_ptr, &new_i);
			free(result);
			result = temp;
			i = new_i;
			continue;
		}
		i++;
	}
	return (result);
}

char *expand_or_not(parser_t *parser, char status)
{
    char *current_arg;
    char *expanded;
	t_env	**env_list_ptr;

	env_list_ptr = init_env(NULL);
	if (!parser || !parser->current || !parser->current->value)
		return(NULL);
	current_arg = parser->current->value;
    if (!should_expand(status))
        return (current_arg);
    if (!env_list_ptr)
        return (ft_strdup(current_arg));
    expanded = expand_all_variables(current_arg, env_list_ptr);
    if (!expanded)
        return (ft_strdup(current_arg));
    return (expanded);
}
