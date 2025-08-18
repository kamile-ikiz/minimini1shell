/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_code.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 19:43:46 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/18 21:11:09 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	current_exit_code(int *exit_code)
{
	static int	current;

	if (!exit_code)
		return (current);
	current = *exit_code;
	return (current);
}

void	set_exit_code(int status)
{
	current_exit_code(&status);
}

int	get_exit_code(void)
{
	return (current_exit_code(NULL));
}
