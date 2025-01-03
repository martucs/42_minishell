/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:36:08 by ineimatu          #+#    #+#             */
/*   Updated: 2024/10/14 16:25:41 by ineimatu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "minishell.h"
# include "struct.h"

int		valid_line(t_info *info);
int		lexer(t_info *info);
int		add_node(char *str, char *token, t_info *info);
int		skip_quote(char *rl, int *i, char quote);
int		handle_quotes(t_info *info, int i);
int		check_token(t_info *info, int i);
int		token_exist(char token);
void	exit_free(char *str, int i, t_info *info);
void	free_lexlst(t_lex_lst *tokens);
int		handle_word(t_info *info, int i);
void	print_lex_lst(t_lex_lst *node);
int		zero_case(t_info *info, int i);
int		word(t_info *info, int i);
int		avoid(char c);

#endif
