/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 15:56:52 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/21 02:25:56 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_expand_data(t_expanded_part *data, char *arg, int dollar_pos)
{
	data->before = ft_substr(arg, 0, dollar_pos);
	data->after = NULL;
	data->result = NULL;
	data->new_pos = 0;
}

static void	set_after_part(t_expanded_part *data, char *arg, int var_end)
{
	data->after = ft_strdup(&arg[var_end]);
}

static void	create_result(t_expanded_part *data, char *env_value)
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

static void	cleanup_expand_data(t_expanded_part *data)
{
	if (data->before)
		free(data->before);
	if (data->after)
		free(data->after);
}

char	*expand_variable_parts(char *arg, char *env_value,
	int dollar_pos, int var_end)
{
	t_expanded_part	data;

	init_expand_data(&data, arg, dollar_pos);
	set_after_part(&data, arg, var_end);
	create_result(&data, env_value);
	cleanup_expand_data(&data);
	return (data.result);
}
