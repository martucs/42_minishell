# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/15 10:41:49 by ineimatu          #+#    #+#              #
#    Updated: 2024/12/01 00:13:07 by martalop         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main.c env_utils.c env_utils2.c lexer_quots.c lexer.c token.c lexer_list.c err_messags.c exec_clean.c exec_utils.c pipex_utils.c syntax_err.c tkn_to_cmd.c parsing_utils.c exec_utils2.c redir_utils.c mini_cd.c mini_echo.c mini_env.c mini_exit.c mini_pwd.c mini_unset.c buitin_utils.c buitin_utils1.c mini_export.c export_utils.c export_utils2.c export_utils3.c export_utils4.c signals.c post_expanse.c expansion.c special_split.c break_down_str.c divide_str_expanse.c specific_expansion.c special_split2.c post_expanse2.c joins_and_quote_rmv.c exec_clean2.c executor.c exit_safe.c

OBJ = $(SRC:.c=.o)

CC = cc

RFLAG = -lreadline

INCLUDES = minishell.h lexer.h struct.h execution.h parsing.h builtin.h signal.h expansion.h libft/libft.h

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
