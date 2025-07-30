
// #include "minishell.h"

// token_t *create_test_token(char *value) {
//     token_t *token = malloc(sizeof(token_t));
//     token->type = TOKEN_WORD;
//     token->value = ft_strdup(value);
//     token->next = NULL;
//     return token;
// }

// parser_t *create_test_parser(char *input) {
//     parser_t *parser = malloc(sizeof(parser_t));
//     parser->tokens = create_test_token(input);
//     parser->current = parser->tokens;
//     parser->inp = ft_strdup(input);
//     parser->pos = 0;
//     parser->error = 0;
//     parser->error_msg = NULL;
//     return parser;
// }

// void free_test_parser(parser_t *parser) {
//     if (parser->tokens) {
//         free(parser->tokens->value);
//         free(parser->tokens);
//     }
//     free(parser->inp);
//     free(parser);
// }

// void test_case(char *input, char status, char *expected, char *test_name) {
//     printf("\n=== %s ===\n", test_name);
//     printf("Input: '%s'\n", input);
//     printf("Status: '%c'\n", status);
//     printf("Expected: '%s'\n", expected);
    
//     parser_t *parser = create_test_parser(input);
//     char *result = expand_or_not(parser, status);
    
//     printf("Got: '%s'\n", result ? result : "NULL");
    
//     if (result && expected && strcmp(result, expected) == 0) {
//         printf("✅ PASSED\n");
//     } else if (!result && !expected) {
//         printf("✅ PASSED\n");
//     } else {
//         printf("❌ FAILED\n");
//     }
    
//     if (result) free(result);
//     free_test_parser(parser);
// }

// int main(void) {
//     printf("=== TESTING expand_or_not FUNCTION ===\n");
    
//     // Test 1: Normal expansion
//     test_case("$USER", '\0/*  */', "testuser", "Normal variable expansion");
    
//     // Test 2: Single quote (no expansion)
//     test_case("$USER", '\'', "$USER", "Single quote - no expansion");
    
//     // Test 3: Double quote (expansion)
//     test_case("$USER", '"', "testuser", "Double quote - with expansion");
    
//     // Test 4: HOME variable
//     test_case("$HOME", '\0', "/home/testuser", "HOME variable expansion");
    
//     // Test 5: PATH variable
//     test_case("$PATH", '\0', "/usr/bin:/bin", "PATH variable expansion");
    
//     // Test 6: Non-existent variable
//     test_case("$NONEXISTENT", '\0', "", "Non-existent variable");
    
//     // Test 7: No dollar sign
//     test_case("normaltext", '\0', "normaltext", "No variable to expand");
    
//     // Test 8: Empty string
//     test_case("", '\0', "", "Empty string");
    
//     // Test 9: Mixed text with variable
//     test_case("Hello$USER", '\0', "Hellotestuser", "Mixed text with variable");
    
//     printf("\n=== TESTING COMPLETED ===\n");
//     return 0;
// }