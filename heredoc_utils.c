/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:30:55 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/31 15:11:43 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_variable_heredoc(char *line, int start, int end)
{
	char	*var_name;
	char	*env_value;
	t_env	**env_list_ptr;

	var_name = ft_substr(line, start, end - start);
	if (!var_name)
		return (ft_strdup(""));
	env_list_ptr = init_env(NULL);
	if (!env_list_ptr)
	{
		free(var_name);
		return (ft_strdup(""));
	}
	env_value = get_env_value(var_name, env_list_ptr);
	free(var_name);
	if (!env_value)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}

char	*append_variable(char *line, int *i, char *result)
{
	int		start;
	char	*temp;

	start = *i + 1;
	while (line[*i + 1] && (ft_isalnum(line[*i + 1]) || line[*i + 1] == '_'))
		(*i)++;
	temp = expand_variable_heredoc(line, start, *i + 1);
	result = ft_strjoin_free(result, temp);
	free(temp);
	return (result);
}

char	*append_normal_char(char *line, int i, char *result)
{
	char	*temp;

	temp = ft_substr(line, i, 1);
	result = ft_strjoin_free(result, temp);
	free(temp);
	return (result);
}


char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

static int	read_to_buffer(int fd, char *buffer, int *buffer_pos)
{
	int	read_size;

	read_size = read(fd, buffer, BUFFER_SIZE);
	*buffer_pos = 0;
	return (read_size);
}

static int	append_char(char *line, int *line_len, char c)
{
	if (*line_len >= 1023)
		return (0);
	line[(*line_len)++] = c;
	return (1);
}

static int	fill_line(int fd, char *line, char *buffer, int *pos_size[2])
{
	int	*pos = pos_size[0];
	int	*size = pos_size[1];

	while (1)
	{
		if (*pos >= *size)
		{
			*size = read_to_buffer(fd, buffer, pos);
			if (*size <= 0)
				break ;
		}
		if (!append_char(line, &pos_size[2][0], buffer[(*pos)++]))
			break ;
		if (line[pos_size[2][0] - 1] == '\n')
			break ;
	}
	return (pos_size[2][0]);
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	static int	buffer_pos = 0;
	static int	buffer_size = 0;
	char		*line;
	int			line_len;
	int			*pos_size[3];

	line = malloc(1024);
	if (!line)
		return (NULL);
	line_len = 0;
	pos_size[0] = &buffer_pos;
	pos_size[1] = &buffer_size;
	pos_size[2] = &line_len;
	if (fill_line(fd, line, buffer, pos_size) == 0)
	{
		free(line);
		return (NULL);
	}
	line[line_len] = '\0';
	return (line);
}

void	cleanup_heredoc_files(void)
{
	system("rm -f /tmp/.heredoc_*");
}