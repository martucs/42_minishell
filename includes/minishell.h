/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ineimatu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 13:00:48 by ineimatu          #+#    #+#             */
/*   Updated: 2024/12/18 20:32:29 by martalop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include "readline/readline.h"
# include "readline/history.h"
# include "lexer.h"
# include "struct.h"
# include "parsing.h"
# include "execution.h"
# include "expansion.h"
# include "builtin.h"
# include "../libft/libft.h"
# include "signal.h"

extern int	g_global;

// initial functions
int		start_reading(t_info *info);
int		init_struct(t_info *info, char **env);

//creating env copy
void	print_env(t_envp *lst);
void	print_char_arr(char **arr);
void	free_array(char **array);
void	free_envlst(t_envp *envp);

int		fill_env_node(char *env_line, t_envp *node, t_envp **env_lst);
void	add_node_to_env(t_envp **lst, t_envp *node);
int		find_equal(char *str);
t_envp	*env_to_list(char **env);
char	**envlst_to_arr(t_envp *envp);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strcpy(char *s1, char *s2);
int		cmdlst_size(t_cmd *cmds);
void	exit_simpl_syntax(t_info *info);
void	exit_lexer(t_info *info);
void	exit_tkn_to_cmd(t_info *info);

#endif
