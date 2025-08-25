/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beysonme@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 18:20:29 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/25 02:54:45 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	int		j;
	char	*ptr;

	i = 0;
	j = 0;
	if (!s1)
		return (NULL);
	while (s1[i] != '\0')
	{
		i++;
	}
	ptr = malloc(sizeof(char) * (i + 1));
	if (!ptr)
		return (0);
	while (s1[j] != '\0')
	{
		ptr[j] = s1[j];
		j++;
	}
	ptr[j] = '\0';
	return (ptr);
}
