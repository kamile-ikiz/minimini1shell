/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_three.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 18:15:42 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/29 18:15:56 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_total_len(char *s1, char *s2, char *s3)
{
	int	len;

	len = 0;
	if (s1)
		len += ft_strlen(s1);
	if (s2)
		len += ft_strlen(s2);
	if (s3)
		len += ft_strlen(s3);
	return (len);
}

static char	*copy_strings(char *result, char *s1, char *s2, char *s3)
{
	int	i;
	int	j;

	i = 0;
	if (s1)
	{
		j = 0;
		while (s1[j])
			result[i++] = s1[j++];
	}
	if (s2)
	{
		j = 0;
		while (s2[j])
			result[i++] = s2[j++];
	}
	if (s3)
	{
		j = 0;
		while (s3[j])
			result[i++] = s3[j++];
	}
	result[i] = '\0';
	return (result);
}

char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*result;
	int		total_len;

	if (!s1 && !s2 && !s3)
		return (ft_strdup(""));
	total_len = get_total_len(s1, s2, s3);
	result = malloc(sizeof(char) * (total_len + 1));
	if (!result)
		return (NULL);
	return (copy_strings(result, s1, s2, s3));
}
