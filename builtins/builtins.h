/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:50:58 by beysonme          #+#    #+#             */
/*   Updated: 2025/08/02 16:30:46 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdbool.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	bool			is_exported;
	bool			is_printed;
}	t_env;

typedef struct command command_t;

int		builtin_cd (command_t *cmd);
int		builtin_echo(command_t *cmd);
int		builtin_pwd(command_t *cmd);
int		builtin_env(command_t *cmd);
int		builtin_export(command_t *cmd);
int		builtin_exit(command_t *cmd);
int		builtin_unset(command_t *cmd);

int		is_builtin(char *cmd);
int		execute_builtin(command_t *cmd);

int		is_valid_identifier(char *str);
int		env_list_size(t_env *list);
int		print_identifier_error(char *identifier);
int		ft_strcmp(const char *s1, const char *s2);

void	add_new_var(t_env **list, char *key, char *value, bool is_exported);
void	reset_print_flags(t_env *list);
void	handle_var_with_value(t_env **list, char *arg, char *eq_pos);
void	handle_var_only(t_env **list, char *key);

t_env	**init_env(char **env);
t_env	*find_env_var(t_env *list, char *key);
t_env	*get_last_list(t_env *list);

#endif
