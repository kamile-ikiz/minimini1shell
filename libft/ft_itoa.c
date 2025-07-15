/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <ikizkamile26@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 20:47:01 by kikiz             #+#    #+#             */
/*   Updated: 2024/10/30 19:36:10 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_call_number_digits(int n)
{
	int	res;

	res = 0;
	if (n <= 0)
	{
		res++;
		n *= -1;
	}
	while ((unsigned int)n > 0)
	{
		res++;
		n = (unsigned int)n / 10;
	}
	return (res);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		size;

	size = ft_call_number_digits(n);
	res = malloc((size +1) * sizeof(char));
	if (!res)
		return (0);
	if (n == 0)
		res[0] = 48;
	if (n < 0)
	{
		res[0] = '-';
		n *= -1;
	}
	res[size] = '\0';
	while ((size - 1) >= 0 && (unsigned int)n > 0)
	{
		res[size -1] = ((unsigned int)n % 10) + '0';
		n = (unsigned int)n / 10;
		size--;
	}
	return (res);
}
