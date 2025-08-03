// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   main.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/05/27 16:06:21 by kikiz             #+#    #+#             */
// /*   Updated: 2025/07/27 15:28:34 by kikiz            ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "minishell.h"

// int main(int argc, char **argv, char **env)
// {
//     char *input;
//     token_t *tokens;
    
//     (void)env; // Suppress unused parameter warning
    
//     // Eğer command line argument varsa onları test et
//     if (argc > 1)
//     {
//         printf("\n%s🚀 MINISHELL TOKEN ANALYZER 🚀%s\n", BOLD, RESET_COLOR);
//         for (int i = 1; i < argc; i++)
//         {
//             printf("\n%s📋 Input %d: %s\"%s\"%s\n", CYAN, i, YELLOW, argv[i], RESET_COLOR);
//             tokens = tokenize(argv[i]);
//             if (tokens)
//             {
//                 print_tokens_fancy(tokens);
//                 free_tokens(tokens);
//             }
//         }
//         return (0);
//     }
    
//     // Interactive mode
//     printf("\n%s🎯 MINISHELL TOKEN ANALYZER - Interactive Mode%s\n", BOLD, RESET_COLOR);
//     printf("%sEnter commands to see token analysis (type 'exit' to quit)%s\n\n", GRAY, RESET_COLOR);
    
//     input = malloc(1024); // Simple buffer for demo
//     while (1)
//     {
//         printf("%sminishell> %s", GREEN, RESET_COLOR);
//         if (!fgets(input, 1024, stdin))
//             break;
            
//         // Remove newline
//         int len = strlen(input);
//         if (len > 0 && input[len-1] == '\n')
//             input[len-1] = '\0';
            
//         // Exit command
//         if (strcmp(input, "exit") == 0)
//             break;
            
//         // Skip empty input
//         if (strlen(input) == 0)
//             continue;
            
//         // Tokenize and display
//         tokens = tokenize(input);
//         if (tokens)
//         {
//             print_tokens_fancy(tokens);
//             printf("\n%s--- Simple view ---%s", GRAY, RESET_COLOR);
//             print_tokens_simple(tokens);
//             free_tokens(tokens);
//         }
//         printf("\n");
//     }
    
//     free(input);
//     printf("\n%sGoodbye! 👋%s\n", YELLOW, RESET_COLOR);
//     return (0);
// }




// #include "minishell.h"

// int main(int ac, char **av, char **env)
// {
// 	(void)ac;
// 	(void)av;
// 	init_env(env);

// 	setup_heredoc_redirect(NULL, "EOF");

// 	return 0;
// }


//#include "minishell.h"

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
// int main(int ac, char **av, char **env)
// {
// 	(void)av;
// 	(void)ac;
// 	parser_t *parser;
// 	parser = malloc(sizeof(parser_t));
// 	parser->current = malloc(sizeof(token_t));
// 	parser->current->value = "$\"HOME\"";
// 	init_env(env);
// 	// command_t *cmd;
// 	// cmd = malloc(sizeof(command_t));  // bellekte yer ayır
// 	// if (!cmd)
//     // 	return (0);  // veya hata yönetimi
// 	// char *env_args[] = {"env", NULL};
// 	// cmd->args = env_args;
// 	// builtin_env(cmd);
// 	// 	command_t *cmd1;
// 	// cmd1 = malloc(sizeof(command_t));  // bellekte yer ayır
// 	// if (!cmd1)
//     // 	return (0);  // veya hata yönetimi
// 	// char *env_args1[] = {"export", "AAA=bbb", NULL};
// 	// cmd1->args = env_args1;/*  */
// 	// builtin_export(cmd1);
// 	// 		command_t *cmd2;
// 	// cmd2 = malloc(sizeof(command_t));  // bellekte yer ayır
// 	// if (!cmd2)
//     // 	return (0);  // veya hata yönetimi
// 	// char *env_args2[] = {"env", NULL};
// 	// cmd2->args = env_args2;
// 	// builtin_env(cmd2); 
	

// 	printf("Expanded value: %s\n", expand_or_not(parser, '\"'));
// 	return 0;
// }