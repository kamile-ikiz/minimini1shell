/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 14:28:47 by kikiz             #+#    #+#             */
/*   Updated: 2025/08/07 17:06:06 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_temp_filename(void)
{
	char	*filename;
	char	*num_str;
	int		temp_num;
	static int	counter;

	counter = 0;

	temp_num = getpid() + counter++;
	num_str = ft_itoa(temp_num);
	if (!num_str)
		return (NULL);
	filename = ft_strjoin("/tmp/.heredoc_", num_str);
	free(num_str);
	return (filename);
}

static char	*expand_heredoc_line(char *line)
{
	char	*result;
	int		i;

	if (!line || !ft_strchr(line, '$'))
		return (ft_strdup(line));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && (ft_isalnum(line[i + 1]) || line[i + 1] == '_'))
			result = append_variable(line, &i, result);
		else
			result = append_normal_char(line, i, result);
		i++;
	}
	return (result);
}

static int	write_heredoc_content(int fd, char *delimiter)
{
	char	*line;
	char	*expanded_line;
	size_t	delim_len;

	delim_len = ft_strlen(delimiter);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delim_len) == 0
			&& (line[delim_len] == '\n' || line[delim_len] == '\0'))
		{
			free(line);
			break ;
		}
		expanded_line = expand_heredoc_line(line);
		write(fd, expanded_line, ft_strlen(expanded_line));
		free(line);
		free(expanded_line);
	}
	return (0);
}

char	*handle_heredoc_delimiter(char *delimiter)
{
	char	*temp_file;
	int		fd;
	int		status;

	if (!delimiter)
		return (NULL);
	temp_file = create_temp_filename();
	if (!temp_file)
		return (NULL);
	fd = open(temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(temp_file);
		return (NULL);
	}
	status = write_heredoc_content(fd, delimiter);
	close(fd);
	if (status == -1)
	{
		unlink(temp_file);
		free(temp_file);
		return (NULL);
	}
	return (temp_file);
}

int	setup_heredoc_redirect(char *delimiter)
{
	char	*temp_file;
	int		fd;

	temp_file = handle_heredoc_delimiter(delimiter);
	if (!temp_file)
		return (-1);
	fd = open(temp_file, O_RDONLY);
	if (fd == -1)
	{
		unlink(temp_file);
		free(temp_file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		unlink(temp_file);
		free(temp_file);
		return (-1);
	}
	close(fd);
	unlink(temp_file);
	free(temp_file);
	return (0);
}
