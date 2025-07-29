/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kikiz <kikiz@student.42istanbul.com.tr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 17:39:33 by kikiz             #+#    #+#             */
/*   Updated: 2025/07/29 18:35:30 by kikiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv,char **env)
{
	(void) argc;
	(void) argv;
	(void) env;
	char *input;
	token_t *tokens;
	while(1)
	{
		input = readline(PROMPT);
		tokens = tokenize(input);
		if (tokens && check_all_syntax(tokens))
        {
            print_tokens_fancy(tokens);
            printf("\n%s--- Simple view ---%s", GRAY, RESET_COLOR);
            print_tokens_simple(tokens);
            free_tokens(tokens);
        }
        printf("\n");
		if (!input) //ctrl+D(EOF)
			break;
		if (*input)
			add_history(input);
		free(input);
	}
	return 0;
}
