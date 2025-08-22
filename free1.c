/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 02:34:53 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/22 08:27:45 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	free_pipeline(t_command *pipeline)
// {
// 	t_command	*current;
// 	t_command	*next;

// 	current = pipeline;
// 	while (current)
// 	{
// 		next = current->next;
// 		free_command(current);
// 		current = next;
// 	}
// }

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void free_environment(t_env **env_list)
{
    t_env *current = *env_list;
    t_env *next;
    
    while (current != NULL)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    *env_list = NULL;
}
