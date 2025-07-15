/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 15:58:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/11 15:58:34 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*all_args(int argc, char **argv)
{
	int		i;
	char	*result;
	char	*tmp;
	char	*tmp2;

	i = 1;
	result = ft_strdup("");
	while (i < argc)
	{
		tmp2 = result;
		tmp = ft_strjoin(argv[i], " ");
		result = ft_strjoin(tmp2, tmp);
		free(tmp);
		free(tmp2);
		i++;
	}
	return (result);
}