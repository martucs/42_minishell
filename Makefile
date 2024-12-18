# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 10:41:49 by ineimatu          #+#    #+#              #
#    Updated: 2024/12/18 21:03:13 by martalop         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main.c env_utils.c env_utils2.c lex_and_pars/lexer_quots.c lex_and_pars/lexer.c lex_and_pars/token.c lex_and_pars/lexer_list.c execution/err_messags.c execution/exec_clean.c execution/exec_utils.c execution/pipex_utils.c syntax_err.c lex_and_pars/tkn_to_cmd.c lex_and_pars/parsing_utils.c execution/exec_utils2.c execution/redir_utils.c builtins/mini_cd.c builtins/mini_echo.c builtins/mini_env.c builtins/mini_exit.c builtins/mini_pwd.c builtins/mini_unset.c builtins/builtin_utils.c builtins/builtin_utils1.c builtins/mini_export.c builtins/export_utils.c builtins/export_utils2.c builtins/export_utils3.c builtins/export_utils4.c signals.c expansion/post_expanse.c expansion/expansion.c expansion/special_split.c expansion/break_down_str.c expansion/divide_str_expanse.c expansion/specific_expansion.c expansion/special_split2.c expansion/post_expanse2.c expansion/joins_and_quote_rmv.c execution/exec_clean2.c execution/executor.c exit_safe.c

OBJ = $(SRC:.c=.o)

CC = cc

RFLAG = -lreadline

INCLUDES = includes/minishell.h includes/lexer.h includes/struct.h includes/execution.h includes/parsing.h includes/builtin.h includes/signal.h includes/expansion.h libft/libft.h

CFLAGS = -Wall -Werror -Wextra -fsanitize=address

LIBFT_A = libft/libft.a

all: makelibft $(NAME)

makelibft: 
	make -C libft

$(NAME): $(LIBFT_A) $(OBJ) Makefile $(INCLUDES)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L./libft -lft $(RFLAG) 

%.o: %.c Makefile $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ)
		make clean -C libft

fclean: clean
		rm -f $(NAME)
		rm -f $(LIBFT_A)

re: fclean all

.PHONY: all clean fclean re
