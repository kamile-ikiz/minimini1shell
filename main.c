/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:06:21 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/20 21:28:59 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **env)
{
    char *input;
    token_t *tokens;
    
    (void)env; // Suppress unused parameter warning
    
    // Eğer command line argument varsa onları test et
    if (argc > 1)
    {
        printf("\n%s🚀 MINISHELL TOKEN ANALYZER 🚀%s\n", BOLD, RESET_COLOR);
        for (int i = 1; i < argc; i++)
        {
            printf("\n%s📋 Input %d: %s\"%s\"%s\n", CYAN, i, YELLOW, argv[i], RESET_COLOR);
            tokens = tokenize(argv[i]);
            if (tokens)
            {
                print_tokens_fancy(tokens);
                free_tokens(tokens);
            }
        }
        return (0);
    }
    
    // Interactive mode
    printf("\n%s🎯 MINISHELL TOKEN ANALYZER - Interactive Mode%s\n", BOLD, RESET_COLOR);
    printf("%sEnter commands to see token analysis (type 'exit' to quit)%s\n\n", GRAY, RESET_COLOR);
    
    while (1)
    {
        input = readline(GREEN "minishell> " RESET_COLOR);
        
        // Handle EOF (Ctrl+D)
        if (!input)
        {
            printf("\n%sGoodbye! 👋%s\n", YELLOW, RESET_COLOR);
            break;
        }
            
        // Exit command
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
            
        // Skip empty input
        if (strlen(input) == 0)
        {
            free(input);
            continue;
        }
        
        // Add to history
        add_history(input);
            
        // Tokenize and display
        tokens = tokenize(input);
        if (tokens)
        {
            print_tokens_fancy(tokens);
            printf("\n%s--- Simple view ---%s", GRAY, RESET_COLOR);
            print_tokens_simple(tokens);
            free_tokens(tokens);
        }
        printf("\n");
        
        free(input);
    }
    
    printf("\n%sGoodbye! 👋%s\n", YELLOW, RESET_COLOR);
    return (0);
}
