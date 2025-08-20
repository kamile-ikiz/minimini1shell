/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:57:26 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/21 01:34:00 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static char	*expand_var(const char *input, size_t *i)
{
	char	*varname;
	char	*env_val;
	char	*value;
	size_t	start;

	// $? özel durumu
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(get_exit_code()));
	}
	// normal değişken adı
	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	varname = ft_substr(input, start, *i - start);
	if (!varname || !*varname)
		return (ft_strdup("")); // $ tek başına gelirse boş string
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

static char	*handle_dollar(const char *input, size_t *i, char *result)
{
	char	*tmp;
	char	*value;

	(*i)++;
	if (input[*i] == '?') // $? özel case
	{
		(*i)++;
		value = ft_itoa(get_exit_code());
		tmp = ft_strjoin(result, value);
		free(value);
		free(result);
		result = tmp;
	}
	else if (ft_isalpha(input[*i]) || input[*i] == '_')
	{
		value = expand_var(input, i);
		tmp = ft_strjoin(result, value);
		free(value);
		free(result);
		result = tmp;
	}
	else
	{
		// sadece $ gelmişse (örn. "echo $" gibi)
		result = append_char(result, '$');
	}
	return (result);
}


char	*expand(const char *input)
{
	size_t	i;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (input[i])
	{
		if (input[i] == '$')
			result = handle_dollar(input, &i, result);
		else
		{
			result = append_char(result, input[i]);
			i++;
		}
	}
	return (result);
}
