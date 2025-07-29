/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beysonme <beyzasonmez565@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 15:50:58 by beysonme          #+#    #+#             */
/*   Updated: 2025/07/26 20:05:19 by beysonme         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdbool.h>

typedef struct s_command
{
	char	**args;
}	t_command;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	bool			is_exported;
	bool			is_printed;
}	t_env;

int		builtin_cd(t_command *cmd);
int		builtin_echo(t_command *cmd);
int		builtin_pwd(t_command *cmd);
int		builtin_env(t_command *cmd);
int		builtin_export(t_command *cmd);
int		builtin_exit(t_command *cmd);
int		builtin_unset(t_command *cmd);

int		is_builtin(char *cmd);
int		execute_builtin(t_command *cmd);

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
