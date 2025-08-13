/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 14:20:00 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/13 13:59:40 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_var(const char *input, size_t *i)
{
	char	*varname;
	char	*env_val;
	char	*value;

	varname = ft_substr(input, *i, 0);
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	free(varname);
	varname = ft_substr(input, *i - ft_strlen(varname), ft_strlen(varname));
	env_val = get_env_value(varname, init_env(NULL));
	if (env_val)
		value = ft_strdup(env_val);
	else
		value = ft_strdup("");
	free(varname);
	return (value);
}

static char	*append_char(char *result, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(result, buf);
	free(result);
	return (tmp);
}

char	*expand(const char *input)
{
	size_t	i;
	char	*result;
	char	*tmp;
	char	*value;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$')
		{
			i++;
			if (ft_isalpha(input[i]) || input[i] == '_')
			{
				value = expand_var(input, &i);
				tmp = ft_strjoin(result, value);
				free(value);
				free(result);
				result = tmp;
			}
			else
				result = append_char(result, '$');
		}
		else
			result = append_char(result, input[i++]);
	}
	return (result);
}