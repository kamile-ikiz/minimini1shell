/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/30 18:27:02 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	main(int argc, char **argv,char **env)
// {
// 	(void) argc;
// 	(void) argv;
// 	(void) env;
// 	char *input;
// 	token_t *tokens;
// 	while(1)
// 	{
// 		input = readline(PROMPT);
// 		tokens = tokenize(input);
// 		if (tokens && check_all_syntax(tokens))
//         {
//             print_tokens_fancy(tokens);
//             printf("\n%s--- Simple view ---%s", GRAY, RESET_COLOR);
//             print_tokens_simple(tokens);
//             free_tokens(tokens);
//         }
//         printf("\n");
// 		if (!input) //ctrl+D(EOF)
// 			break;
// 		if (*input)
// 			add_history(input);
// 		free(input);
// 	}
// 	return 0;
// }
int main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	parser_t *parser;
	parser = malloc(sizeof(parser_t));
	parser->current = malloc(sizeof(token_t));
	parser->current->value = "gjfhg$USER rdtfyguhjk";
	init_env(env);
	// t_command *cmd;
	// cmd = malloc(sizeof(t_command));  // bellekte yer ayır
	// if (!cmd)
    // 	return (0);  // veya hata yönetimi
	// char *env_args[] = {"env", NULL};
	// cmd->args = env_args;
	// builtin_env(cmd);
	// 	t_command *cmd1;
	// cmd1 = malloc(sizeof(t_command));  // bellekte yer ayır
	// if (!cmd1)
    // 	return (0);  // veya hata yönetimi
	// char *env_args1[] = {"export", "AAA=bbb", NULL};
	// cmd1->args = env_args1;
	// builtin_export(cmd1);
	// 		t_command *cmd2;
	// cmd2 = malloc(sizeof(t_command));  // bellekte yer ayır
	// if (!cmd2)
    // 	return (0);  // veya hata yönetimi
	// char *env_args2[] = {"env", NULL};
	// cmd2->args = env_args2;
	// builtin_env(cmd2); 
	

	printf("Expanded value: %s\n", expand_or_not(parser, '\"'));
	return 0;
}