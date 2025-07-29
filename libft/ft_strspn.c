/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 13:08:49 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/26 13:14:51 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(const char *str1, const char *str2)
{
	size_t		i;
	const char	*p;
	int			found;

	i = 0;
	while (str1[i])
	{
		p = str2;
		found = 0;
		while (*p)
		{
			if (str1[i] == *p)
			{
				found = 1;
				break ;
			}
			p++;
		}
		if (!found)
			break ;
		i++;
	}
	return (i);
}
