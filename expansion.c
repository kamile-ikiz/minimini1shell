/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 16:19:04 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/30 18:26:13 by kikiz            ###   ########.fr       */
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

static char *get_env_value(char *var_name, t_env **env_list_ptr)
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

static char *expand_variable(char *arg, t_env **env_list_ptr)
{
	char	*var_name;
	char	*env_value;
	char	*result;
	char	*before;
	char	*after;
	int		dollar_pos;
	int		var_end;

	dollar_pos = 0;
	while (arg[dollar_pos] && arg[dollar_pos] != '$')
		dollar_pos++;
	if(!arg[dollar_pos])
		return (ft_strdup(arg));
	var_name = extract_var_name(arg, dollar_pos, &var_end);
	if (!var_name)
		return (ft_strdup(arg));
	env_value = get_env_value(var_name, env_list_ptr);
	before = ft_substr(arg, 0, dollar_pos);
	after = ft_strdup(&arg[var_end]);
	if (!env_value)
	{
		var_name = ft_strjoin("$", var_name);
		result = ft_strjoin_three(before, var_name, after);
	}
	else
		result = ft_strjoin_three(before, env_value, after);
	free(var_name);
	free(before);
	free(after);
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
    expanded = expand_variable(current_arg, env_list_ptr);
    if (!expanded)
        return (ft_strdup(current_arg));
    return (expanded);
}
