// void print_parser_error(t_parser *parser)
// {
//     if (parser && parser->error)
//     {
//         if (parser->error_msg)
// 		{
// 			ft_putstr_fd(parser->tokens->value, 2);
// 			ft_putendl_fd(parser->error_msg, 2);
// 		}
//         else
// 			ft_putendl_fd("Parser error: Unknown error", 2);
//     }
// }

// void free_parser(t_parser *parser)
// {
//     if (!parser)
//         return;
//     free_tokens(parser->tokens);
//     if (parser->inp)
//         free(parser->inp);
//     if (parser->error_msg)
//         free(parser->error_msg);
//     free(parser);
// }