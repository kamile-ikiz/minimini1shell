// debug_print.c

#include "minishell.h"

// Yönlendirme türünü okunabilir bir string'e çevirir
static const char	*redirect_type_to_str(token_type_t type)
{
	if (type == TOKEN_REDIRECT_IN)
		return ("< (IN)");
	if (type == TOKEN_REDIRECT_OUT)
		return ("> (OUT)");
	if (type == TOKEN_REDIRECT_APPEND)
		return (">> (APPEND)");
	if (type == TOKEN_HEREDOC)
		return ("<< (HEREDOC)");
	return ("UNKNOWN");
}

// Bir komutun yönlendirme listesini yazdırır
static void	print_redirects(redirect_t *redirects)
{
	redirect_t	*current;
	int			i;

	current = redirects;
	i = 0;
	while (current)
	{
		printf("    %s[Redirect %d]%s\n", YELLOW, i, RESET_COLOR);
		printf("      Type: %s\n", redirect_type_to_str(current->type));
		printf("      File/Delimiter: %s\n", current->filename);
		current = current->next;
		i++;
	}
}

// Bir pipeline'daki tüm komutları ve detaylarını yazdırır
void	print_pipeline(command_t *pipeline_head)
{
	command_t	*current_cmd;
	int			i;
	int			j;

	if (!pipeline_head)
	{
		printf(RED "Parser 'dan boş pipeline döndü.\n" RESET_COLOR);
		return ;
	}
	printf(CYAN BOLD "---------- PARSER SONUCU ----------\n" RESET_COLOR);
	current_cmd = pipeline_head;
	i = 0;
	while (current_cmd)
	{
		printf(GREEN "[Komut %d]\n" RESET_COLOR, i);
		printf("  Argümanlar (%d adet):\n", current_cmd->argc);
		j = 0;
		while (j < current_cmd->argc)
		{
			printf("    args[%d]: \"%s\"\n", j, current_cmd->args[j]);
			j++;
		}
		print_redirects(current_cmd->redirects);
		current_cmd = current_cmd->next;
		i++;
		if (current_cmd)
			printf(BLUE "  |\n  V\n" RESET_COLOR);
	}
	printf(CYAN BOLD "-----------------------------------\n" RESET_COLOR);
}